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

   TelnetDaemon

        HISTORY:
                19.06.2002 MK
                        - initial version
*/
#ifndef ZEITGEIST_TELNETDAEMON_H
#define ZEITGEIST_TELNETDAEMON_H

#include <list>
#include <memory>
#include <rcssnet/addr.hpp>
#include <rcssnet/tcpsocket.hpp>
#include <string>
#include <zeitgeist/corecontext.h>

namespace zeitgeist
{

class TelnetSession;

/** the TelnetDaemon is responsible to listen for incoming
    connections. For each connection it creates a new thread with a
    TelnetSession object, managing the session.
*/
class TelnetDaemon
{
    //
    // types
    //
public:
protected:
private:
    typedef std::list<TelnetSession*>       TSessionList;

    //
    // functions
    //
public:
    /** constructs the TelnetDaemon
        \param port the port number to listen on
        \param hostname the hostname of this machine
        \param coreContext a reference to a core context
    */
    TelnetDaemon(int port, std::string hostName, std::shared_ptr<CoreContext> coreContext);
    virtual ~TelnetDaemon();

    /** contains the code run inside the thread for this Daemon
        \param self a shared pointer to this object
    */
    void Run(std::shared_ptr<TelnetDaemon> self);

    /** called from the server to shutdown the daemon. This method
        destroys all session objects */
    void Terminate();

    /** prints the status of the daemon to stdout */
    void Status();

    /** adds a session a object to the list of managed sessions */
    void Attach(TelnetSession *session);

    /** removes a session object from the list of managed sessions */
    void Detach(TelnetSession *session);

    /** returns the hostname of this machine */
    const std::string& GetHostName() const { return mHostName; }

protected:
    /** create the network socket and start the daemon */
    bool Init();


    /** accepts a pending connection request, creates a new socket
        and associates the TelnetSession cc with it*/
    bool AcceptConnection(TelnetSession& cc);

    //
    // members
    //
public:
protected:
private:
    /** the listen socket of the server */
    rcss::net::TCPSocket mDaemonSocket;

    /** the local adress the daemon is bound to */
    rcss::net::Addr mDaemonAddr;

    /** the hostname of this machine */
    std::string mHostName;

    /** the list of current active sessions */
    TSessionList    mSessions;

    /** reference to the zeitgeist core */
    std::shared_ptr<CoreContext> mCoreContext;
};

}

#endif // ZEITGEIST_TELNETDAEMON_H
