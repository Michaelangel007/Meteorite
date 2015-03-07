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

namespace Meteorite
{
	class Application;

	namespace Network
	{
		class Client;
		class Protocol;

		class NetworkManager
		{
		public:
			NetworkManager(shared_ptr<Meteorite::Application> app);
			~NetworkManager();

			/*
			 * Initialises the network manager.
			 * Returns false if any errors occurred.
			 */
			bool initialise();

			/*
			 * Begins waiting for clients.
			 */
			void start();

			/*
			 * Updates the network manager; should be called every tick.
			 */
			void run();

			/*
			 * Safely stops the network manager.
			 */
			void stop();

			/*
			 * Returns a reference to network manager's boost::io_service object.
			 */
			boost::asio::io_service& getService();

		private:
			shared_ptr<Meteorite::Application> app;

			boost::asio::io_service service;
			shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
			shared_ptr<Meteorite::Network::Protocol> protocol;

			/*
			 * Called when a new client has been accepted.
			 */
			void handleAccept(shared_ptr<Meteorite::Network::Client> client, const boost::system::error_code& error);
		};
	}
}