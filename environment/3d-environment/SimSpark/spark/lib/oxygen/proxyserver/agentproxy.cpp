/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Thu Dec 31 2009
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004-2009 RoboCup Soccer Server 3D Maintenance Group
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
#include "agentproxy.h"
#include <chrono>
#include <rcssnet/exception.hpp>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/simulationserver/netcontrol.h>
#include <errno.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace rcss::net;

AgentProxy::AgentProxy(int cycleMillisecs) : Node(),
    mCycleMillisecs(cycleMillisecs), mFinished(false),
    mAgentBuffer(new NetBuffer)
{
}

AgentProxy::~AgentProxy()
{
}

void AgentProxy::Start(std::shared_ptr<rcss::net::Socket> agentSocket,
           rcss::net::Addr serverAddress)
{
    try
    {
        mAgentSocket = agentSocket;
        mServerSocket = NetControl::CreateSocket(NetControl::ST_TCP);
        if (!mServerSocket)
        {
            mFinished = true;
            return;
        }
        GetLog()->Normal() << "(AgentProxy) '" << GetName() << "'connecting to "
                << serverAddress << "\n";
        mServerSocket->connect(serverAddress);
        if (mServerSocket->isConnected())
        {
            cout << "(AgentProxy) '" << GetName() << "' connected successfully"
                    << endl;
        }

        // assure that a NetMessage object is registered
        mNetMessage = FindChildSupportingClass<NetMessage>();
        if (mNetMessage.get() == 0)
        {
            mNetMessage = std::shared_ptr<NetMessage>(new NetMessage());
        }

        mAgentConnectionThread = std::thread(
            &AgentProxy::AgentConnectionHandler, this);
        mServerConnectionThread = std::thread(
            &AgentProxy::ServerConnectionHandler, this);
        return;
    }
    catch (const BindErr& error)
    {
        GetLog()->Error() << "(AgentProxy) '" << GetName()
                << "' failed to bind socket with '" << error.what() << "'"
                << endl;
    }
    catch (const ConnectErr& error)
    {
        GetLog()->Error() << "(AgentProxy) '" << GetName()
                << "' connection failed with: '" << error.what() << "'" << endl;
    }

    // executed on error
    mServerSocket->close();
    mFinished = true;
}

void AgentProxy::Stop()
{
    mFinished = true;
    mServerConnectionThread.join();
}

void AgentProxy::ServerConnectionHandler()
{
    string syncMsg("(sync)");
    string servermsg, agentmsg;
    
    std::chrono::time_point<std::chrono::steady_clock> cycleFinishTime = std::chrono::steady_clock::now()
            + std::chrono::milliseconds(mCycleMillisecs);
    std::shared_ptr<NetBuffer> netbuf(new NetBuffer);
    TRawBuffer recvbuf;

    mNetMessage->PrepareToSend(syncMsg);
    while (!mFinished)
    {
        servermsg.clear();
        std::this_thread::sleep_until(cycleFinishTime);

        std::unique_lock agentBufLock(mAgentBufferMutex);
        while (!mAgentBuffer->IsEmpty() &&
                mNetMessage->Extract(mAgentBuffer, agentmsg))
        {
            mNetMessage->PrepareToSend(agentmsg);
            mServerSocket->send(agentmsg.data(), agentmsg.size());
        }
        agentBufLock.unlock();

        mServerSocket->send(syncMsg.data(), syncMsg.size());
        do
        {
            int retval = mServerSocket->recv(recvbuf.data(), recvbuf.size());
            if (retval > 0)
                netbuf->AddFragment(string(recvbuf.data(), recvbuf.size()));
            else
            {
                GetLog()->Error()
                    << "(AgentProxy) ERROR: '" << GetName()
                    << "' recv returned error on a client socket '"
                    << strerror(errno) << "' " << endl;
                mFinished = true;
                break;
            }
        } while (!mNetMessage->Extract(netbuf, servermsg));

        if (!servermsg.empty())
        {
            cycleFinishTime = std::chrono::steady_clock::now()
                    + std::chrono::milliseconds(mCycleMillisecs);

            mNetMessage->PrepareToSend(servermsg);
            mAgentSocket->send(servermsg.data(), servermsg.size(),
                Socket::DONT_CHECK);
        }
    }
    mServerSocket->close();
    mFinished = true;
    mAgentConnectionThread.join();
}

void AgentProxy::AgentConnectionHandler()
{
    TRawBuffer recvbuf;

    while (!mFinished)
    {
        int retval = mAgentSocket->recv(recvbuf.data(), recvbuf.size());
        if (retval > 0)
        {
            std::lock_guard agentBufLock(mAgentBufferMutex);
            mAgentBuffer->AddFragment(string(recvbuf.data(), recvbuf.size()));
        }
        else if (retval <= 0 && errno != EAGAIN)
        {
            GetLog()->Error() << "(AgentProxy) ERROR: '" << GetName()
                    << "' recv returned error on a client socket '"
                    << strerror(errno) << "' " << endl;
            mFinished = true;
        }
    }
}
