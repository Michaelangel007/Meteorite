/*
 * Copyright (C) 2015 Alex Rathbone
 *
 * This file is part of Meteorite.
  
 * Meteorite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
  
 * Meteorite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
  
 * You should have received a copy of the GNU General Public License
 * along with Meteorite.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Meteorite/Application.h>

#include <Meteorite/Network/NetworkManager.h>

namespace Meteorite
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	bool Application::initialise(int argc, char** argv)
	{
		if (initialised)
			return false;

		// Initialise the Logger
		Meteorite::LoggerSettings loggerSettings;
#ifdef METEORITE_DEBUG
		loggerSettings.filePath = "./meteorite_d.log";
#else
		loggerSettings.filePath = "./meteorite.log";
#endif
		loggerSettings.enableTimestamps = true;
		loggerSettings.outputToFile = true;
		loggerSettings.outputToStdOut = true;

		logger = make_shared<Meteorite::Logger>(loggerSettings);
		
		if (!logger->initialise())
			return false;

		logger->info(METEORITE_VERSION_STRING);
		logger->lineBreak();

		// Initialise the NetworkManager
		networkManager = make_shared<Meteorite::Network::NetworkManager>(shared_from_this());
		logger->info("Initialising NetworkManager..");

		if (!networkManager->initialise())
			return false;
		
		logger->info("NetworkManager initialised.");

		initialised = true;
		return true;
	}

	int Application::start()
	{
		if (!initialised || running)
			return 1;

		// Start components
		networkManager->start();

		running = true;
		
		while (running)
		{
			run();
		}

		logger->info("Stopping Meteorite..");
		networkManager->stop();

		return 0;
	}

	void Application::run()
	{
		networkManager->run();
	}

	void Application::stop()
	{
		running = false;
	}

	shared_ptr<Meteorite::Logger> Application::getLogger()
	{
		return logger;
	}

	shared_ptr<Meteorite::Network::NetworkManager> Application::getNetworkManager()
	{
		return networkManager;
	}
}