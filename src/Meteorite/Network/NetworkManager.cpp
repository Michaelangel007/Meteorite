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

#include <Meteorite/Network/NetworkManager.h>

#include <Meteorite/Application.h>
#include <Meteorite/Network/Client.h>
#include <Meteorite/Network/Protocol.h>
#include <Meteorite/Game/WorldManager.h>

namespace Meteorite
{
	namespace Network
	{
		NetworkManager::NetworkManager(shared_ptr<Meteorite::Application> app) :
			app(app)
		{
		}

		NetworkManager::~NetworkManager()
		{
		}

		bool NetworkManager::initialise()
		{
			acceptor = make_shared<boost::asio::ip::tcp::acceptor>(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7777));
			protocol = make_shared<Meteorite::Network::Protocol>(app);

			return true;
		}

		void NetworkManager::start()
		{
			auto client = make_shared<Meteorite::Network::Client>(app, protocol);

			acceptor->async_accept(client->getSocket(),
				boost::bind(&Meteorite::Network::NetworkManager::handleAccept, this, client, boost::asio::placeholders::error));
		}

		void NetworkManager::run()
		{
			service.poll();
		}

		void NetworkManager::stop()
		{
			app->getLogger()->info("Stopping NetworkManager..");
		}

		void NetworkManager::handleAccept(shared_ptr<Meteorite::Network::Client> client, const boost::system::error_code& error)
		{
			if (!error)
			{
				std::string ip = client->getSocket().remote_endpoint().address().to_v4().to_string();
				app->getLogger()->info(boost::str(boost::format("Incoming connection from %1%") % ip));

				int playerId = app->getWorldManager()->addNewPlayer(client);

				if (playerId == -1)
				{
					// TODO: FatalError response
					app->getLogger()->error("Server full, could not accept client.");
					client->stop();
				}
				else
				{
					app->getLogger()->info(boost::str(boost::format("Assigned player ID %1%") % (int) playerId));
					client->start();
				}
			}
			else
			{
				app->getLogger()->error("Failed to accept incoming client!");
				app->getLogger()->error(boost::str(boost::format("Reason: %1%") % error.message()));
			}

			// wait for another client
			start();
		}

		boost::asio::io_service& NetworkManager::getService()
		{
			return service;
		}
	}
}