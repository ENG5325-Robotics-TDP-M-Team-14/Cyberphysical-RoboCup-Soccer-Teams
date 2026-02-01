/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "rubywrapper.h"
#include <utility>

using namespace zeitgeist;

RubyWrapper::RubyWrapper() : mTerminateRubyThread(false)
{
  // Start Ruby thread
  mRubyThread = std::thread(&RubyWrapper::Run, this);
}

RubyWrapper::~RubyWrapper()
{
  // Terminate Ruby thread
  RequestRubyExecution([this]
  {
    mTerminateRubyThread = true;
    return GCValue();
  });
  mRubyThread.join();
}

ScriptValue RubyWrapper::RbEvalStringWrap(const std::string& str, int& error)
{
  return RequestRubyExecution([&str, &error]
  {
    return UnsafeRubyWrapper::RbEvalStringWrap(str, error);
  });
}

ScriptValue RubyWrapper::RbEvalStringWrap(const std::string& str)
{
  int error;
  return RbEvalStringWrap(str, error);
}

ScriptValue RubyWrapper::RbConstGet(const std::string& name)
{
  return RequestRubyExecution([&name]
  {
    return rb_const_get(rb_cObject, rb_intern(name.c_str()));
  });
}

ScriptValue RubyWrapper::CallMethod(const std::string& className, const std::string& methodName)
{
  return RequestRubyExecution([&className, &methodName]
  {
    return UnsafeRubyWrapper::CallMethod(className, methodName);
  });
}

ScriptValue RubyWrapper::RequestRubyExecution(std::function<GCValue()> request)
{
  if (std::this_thread::get_id() == mRubyThread.get_id())
  {
    // Already on the Ruby thread
    // Acquiring the mutex would not work.
    // Execute the request directly
    return request().AsScriptValue();
  }

  std::packaged_task<ScriptValue()> packagedRequest(
    [&request]{return request().AsScriptValue();}
  );
  std::future<ScriptValue> future = packagedRequest.get_future();

  // Submit request
  {
    std::lock_guard lock(mRubyRequestsMutex);
    mRubyRequests.push_back(std::move(packagedRequest));
  }
  mRubyRequestNotifier.notify_all();

  return future.get();
}

void RubyWrapper::Run()
{
  ruby_init();

  while(!mTerminateRubyThread)
  {
    std::vector<std::packaged_task<ScriptValue()>> requests;

    // Wait for next request
    {
      std::unique_lock lock(mRubyRequestsMutex);
      mRubyRequestNotifier.wait(lock, [this]{return !mRubyRequests.empty();});
      requests.swap(mRubyRequests);
    }

    // Execute requests
    for (auto &r: requests)
    {
      r();
    }
  }

  ruby_finalize();
}
