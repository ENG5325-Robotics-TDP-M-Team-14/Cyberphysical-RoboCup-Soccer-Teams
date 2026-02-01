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
#ifndef ZEITGEIST_RUBYWRAPPER_H
#define ZEITGEIST_RUBYWRAPPER_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

#include "private/gcvalue.h"
#include "private/unsaferubywrapper.h"
#include "scriptvalue.h"

namespace zeitgeist
{

/** A thread safe wrapper for the Ruby API */
class RubyWrapper
{
public:
    /** constructs the RubyWrapper and starts the Ruby thread */
    RubyWrapper();

    /** destructs the RubyWrapper and joins the Ruby thread */
    ~RubyWrapper();

    /** calls a safe rb_eval_string variant and prints any ruby error
        messages along with a backtrace to stdout
     */
    ScriptValue RbEvalStringWrap(const std::string& str);

    /** calls a safe rb_eval_string variant and prints any ruby error
        messages along with a backtrace to stdout. The error code
        returned from ruby is stored in the 'error' parameter.
     */
    ScriptValue RbEvalStringWrap(const std::string& str, int& error);

    /** returns the constant identified by the supplied name */
    ScriptValue RbConstGet(const std::string& name);

    /** calls a method on the given class */
    ScriptValue CallMethod(const std::string& className, const std::string& methodName);

    /** defines a global function with the given name */
    template<typename... Args> void DefineGlobalFunction(const std::string& name, VALUE (*func) (Args...))
    {
        RequestRubyExecution([&name, func]
        {
            UnsafeRubyWrapper::DefineGlobalFunction(name, func);
            return GCValue();
        });
    }

private:
    /**
     * wrapper function to hand the execution of the given function
     * over to the Ruby thread
     */
    ScriptValue RequestRubyExecution(std::function<GCValue()> request);

    /** the main function for the Ruby thread executing all Ruby code */
    void Run();

    /** the thread that executes all Ruby code */
    std::thread mRubyThread;

    /** a flag to signal the termination of the Ruby thread */
    bool mTerminateRubyThread;

    /** the functions to execute on the Ruby thread */
    std::vector<std::packaged_task<ScriptValue()>> mRubyRequests;

    /** a mutex to protect mRubyRequests */
    std::mutex mRubyRequestsMutex;

    /** condition variable to wake the Ruby thread up on a request */
    std::condition_variable mRubyRequestNotifier;
};
}

#endif // ZEITGEIST_RUBYWRAPPER_H

