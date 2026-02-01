#include "telnetserver.h"
#include <iostream>
#include <thread>
#include "telnetdaemon.h"
#include "../logserver/logserver.h"

#if HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#elif defined(WIN32)
#include <winsock2.h>
#endif

using namespace zeitgeist;

TelnetServer::TelnetServer(unsigned int port) : Node(), mPort(port), mInitialStartComplete(false)
{
	char buffer[512];
	int tmp = gethostname(buffer, 511);
	if (tmp != 0)
	{
		GetLog()->Error() << "(TelnetServer) unable to get hostname\n";
	} else {
		mHostName = buffer;
	}
}

void TelnetServer::OnLink()
{
	if (!mInitialStartComplete)
	{
		Start();
		mInitialStartComplete = true;
	}
}

TelnetServer::~TelnetServer()
{
	Shutdown();
}

bool TelnetServer::Start()
{
	if (mDaemon.get() != nullptr)
	{
		Shutdown();
	}

	// here we start the actual worker thread
	mDaemon = std::make_shared<TelnetDaemon>(mPort, mHostName, GetCore()->CreateContext());
	std::thread daemonThread(&TelnetDaemon::Run, mDaemon, mDaemon);
	daemonThread.detach();

	return true;
}

bool TelnetServer::Shutdown()
{
	if (mDaemon != nullptr)
	{
		mDaemon->Terminate();
		mDaemon.reset();
	}
	return true;
}

void TelnetServer::Status()
{
	std::cout << "TelnetServer::Status()\n";

	if (mDaemon.get() == nullptr)
	{
		std::cout << "  No daemon running...\n";
		return;
	}

	std::cout << "  Daemon running on port " << GetPort() << std::endl;
	
	mDaemon->Status();

	std::cout << std::endl;
}