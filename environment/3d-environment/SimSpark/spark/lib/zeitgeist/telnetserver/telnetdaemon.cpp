#include "telnetdaemon.h"
#include "telnetsession.h"
#include <iostream>
#include <thread>
#include <rcssnet/exception.hpp>
#include <vector>
#include <zeitgeist/core.h>

#if defined(HAVE_SYS_SOCKET_H)
#include <sys/socket.h>
#elif defined(WIN32)
#include <winsock2.h>
#endif

using namespace zeitgeist;

TelnetDaemon::TelnetDaemon(
	int port,
	std::string hostName,
	std::shared_ptr<CoreContext> coreContext
) :	mHostName(hostName),
	mCoreContext(coreContext)
{
	mDaemonAddr = rcss::net::Addr(port);
}

TelnetDaemon::~TelnetDaemon()
{
	Terminate();
}

/*
  The actual thread routine.
*/
void TelnetDaemon::Run(std::shared_ptr<TelnetDaemon> self)
{
	// at this point the bidirectional connection between client/server is
	// established. Now we start the listening socket of the daemon

	if (Init() == false) return;

	std::cout << mHostName << ": Daemon started, listening for connections..." << std::endl;

	std::vector<std::thread> sessionThreads;

	TelnetSession cc(self);

	while (AcceptConnection(cc))
	{
		sessionThreads.emplace_back(cc);
	}

	// we wait for all session threads to finish, before we finish
	for (std::thread& thrd : sessionThreads)
	{
		thrd.join();
	}
}

bool TelnetDaemon::Init()
{
	// create socket
	mDaemonSocket = rcss::net::TCPSocket();
	if (mDaemonSocket.setReuseAddr(true) != 0)
	{
		std::cout << "(TelnetDaemon) failed to reuse address" << std::endl;
	}

	try
	{
		mDaemonSocket.bind(mDaemonAddr);
	}
	catch (const rcss::net::BindErr& error)
	{
		std::cout
			<< "(TelnetDaemon) failed to bind socket: "
			<< error.what() << std::endl;

		mDaemonSocket.close();
		return false;
	}

	// change to passive socket
	if (!mDaemonSocket.listen(SOMAXCONN))
	{
		std::cout << "(TelnetDaemon) listen failed" << std::endl;
		mDaemonSocket.close();
		return false;
	}

	return true;
}

void TelnetDaemon::Terminate()
{
	mDaemonSocket.close();
}

bool TelnetDaemon::AcceptConnection(TelnetSession &cc)
{
	try
	{
		rcss::net::Addr addr;
		std::shared_ptr<rcss::net::Socket> socket(mDaemonSocket.accept(addr));
		if (socket.get() == nullptr)
		{
			std::cout << "(TelnetDaemon) failed to accept connection" << std::endl;
			return false;
		}
		std::cout << "Incoming connection from: " << addr.getHostStr() << std::endl;

		// create a clientconnection
		cc.Init(socket, addr, mCoreContext->GetCore()->GetScriptServer());
	}
	catch (const rcss::net::AcceptErr& error)
	{
		std::cout
			<< "(TelnetDaemon) failed to accept TCP connection: "
			<< error.what() << std::endl;
		return false;
	}

	return true;
}

void TelnetDaemon::Attach(TelnetSession *session)
{
	std::cout << "Attaching TelnetSession " << (void*) session << std::endl;
	mSessions.push_back(session);
}

void TelnetDaemon::Detach(TelnetSession *session)
{
	std::cout << "Detaching TelnetSession " << (void*) session << std::endl;
	mSessions.remove(session);
}

void TelnetDaemon::Status()
{
	std::cout << "  Number of clients: " << mSessions.size() << std::endl;
}