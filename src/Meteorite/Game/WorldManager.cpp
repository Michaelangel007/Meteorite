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

#include <Meteorite/Game/WorldManager.h>

#include <Meteorite/Application.h>
#include <Meteorite/Game/Player.h>
#include <Meteorite/Network/Client.h>
#include <Meteorite/Network/Message.h>
#include <Meteorite/Network/Protocol.h>
#include <Meteorite/Network/Message/All.h>

namespace Meteorite
{
	namespace Game
	{
		WorldManager::WorldManager(shared_ptr<Meteorite::Application> app) :
			app(app)
		{
		}

		WorldManager::~WorldManager()
		{
		}

		bool WorldManager::initialise()
		{
			for (uint8_t id = 0; id < MAX_PLAYERS; ++id)
				players[id] = nullptr;

			addMessageHandler(Meteorite::Network::CMSG_CONNECT_REQUEST, boost::bind(&WorldManager::onConnectRequest, shared_from_this(), _1, _2));

			return true;
		}

		void WorldManager::run()
		{
			for (auto it = players.begin(); it != players.end(); ++it)
			{
				auto player = it->second;

				if (!player)
					continue;

				auto client = player->getClient();

				// Check if client is dead
				if (!client->isAlive())
				{
					app->getLogger()->info(boost::str(boost::format("Removing dead client, ID: %1%") % (int) player->getPlayerSlot()));
					players[player->getPlayerSlot()] = nullptr;
					continue;
				}

				// Check if client was kicked, but didn't close the socket
				if (player->getState() == Player::STATE_KICKED)
				{
					players[player->getPlayerSlot()] = nullptr;
					client->stop();
					continue;
				}

				// Handle incoming messages
				for (auto it = client->getIncoming().begin(); it != client->getIncoming().end(); ++it)
				{
					auto message = *it;

					if (message != nullptr)
						messageHandlers[message->id](player, message);
				}

				client->getIncoming().clear();

				player->run();
				client->write();
			}
		}
		
		int WorldManager::addNewPlayer(shared_ptr<Meteorite::Network::Client> client)
		{
			int availableId = -1;

			for (int id = 0; id < MAX_PLAYERS; ++id)
			{
				if (players[id] == nullptr)
				{
					availableId = id;
					break;
				}
			}

			if (availableId == -1)
				return -1;

			auto player = make_shared<Meteorite::Game::Player>(app, client, availableId);
			players[(uint8_t) availableId] = player;

			return availableId;
		}

		boost::signals2::connection WorldManager::addMessageHandler(uint8_t messageId, const MessageHandler::slot_type& slot)
		{
			return messageHandlers[messageId].connect(slot);
		}

		void WorldManager::onConnectRequest(shared_ptr<Meteorite::Game::Player> player, shared_ptr<Meteorite::Network::Message> message)
		{
			if (player->getState() != Player::STATE_DEFAULT)
				return;

			auto msg = (Meteorite::Network::Message_ConnectRequest*) message.get();

			app->getLogger()->info(boost::str(boost::format("Received connection request from ID: %1%") % (int) player->getPlayerSlot()));
			app->getLogger()->info("Returning SMSG_FATAL_ERROR, with message 'Meteorite Server'.");

			player->kick("Meteorite Server");
		}
	}
}