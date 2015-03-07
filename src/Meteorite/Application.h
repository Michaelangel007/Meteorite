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

#pragma once

#include <Meteorite/Common.h>

#define METEORITE_VERSION_MAJOR			0
#define METEORITE_VERSION_MINOR			0
#define METEORITE_VERSION_BUILD			1
#define METEORITE_VERSION_BUILD_TYPE	"DEVELOPMENT"
#define METEORITE_VERSION_STRING \
	boost::str(boost::format("Meteorite v%1%.%2%.%3%-%4%") % METEORITE_VERSION_MAJOR % METEORITE_VERSION_MINOR % METEORITE_VERSION_BUILD \
		% METEORITE_VERSION_BUILD_TYPE)

namespace Meteorite
{
	namespace Network
	{
		class NetworkManager;
	}

	namespace Game
	{
		class WorldManager;
	}

	class Application : public enable_shared_from_this<Meteorite::Application>
	{
	public:
		Application();
		~Application();

		/*
		 * Initialises all components of the Meteorite application.
		 * Returns false if any errors occurred.
		 */
		bool initialise(int argc, char** argv);

		/*
		 * Starts the application, and returns the exit-code to be used.
		 * Application must be initialise()'d first.
		 */
		int start();

		/*
		 * Main application loop.
		 */
		void run();

		/*
		 * Safely stops the application.
		 */
		void stop();

		/*
		 * Returns a smart pointer to the application's logger.
		 */
		shared_ptr<Meteorite::Logger> getLogger();

		/*
		 * Returns a smart pointer to the application's network manager.
		 */
		shared_ptr<Meteorite::Network::NetworkManager> getNetworkManager();

		/*
		 * Returns a smart pointer to the application's world manager.
		 */
		shared_ptr<Meteorite::Game::WorldManager> getWorldManager();

	private:
		bool initialised = false, running = false;

		shared_ptr<Meteorite::Logger> logger;
		shared_ptr<Meteorite::Network::NetworkManager> networkManager;
		shared_ptr<Meteorite::Game::WorldManager> worldManager;
	};
}