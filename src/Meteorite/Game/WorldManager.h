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

#include <boost/signals2.hpp>

#define MAX_PLAYERS 255 // valid IDs 0-254

namespace Meteorite
{
	class Application;

	namespace Network
	{
		class Client;
		struct Message;
		struct Message_ConnectRequest;
	}

	namespace Game
	{
		class Player;

		class WorldManager : public enable_shared_from_this<Meteorite::Game::WorldManager>
		{
		public:
			typedef boost::signals2::signal<void(shared_ptr<Meteorite::Game::Player> player, shared_ptr<Meteorite::Network::Message>)> MessageHandler;

			WorldManager(shared_ptr<Meteorite::Application> app);
			~WorldManager();

			/*
			 * Initialises the game manager.
			 * Returns false if any errors occurred.
			 */
			bool initialise();

			/*
			 * Updates the world manager; should be called every tick.
			 */
			void run();

			/*
			 * Creates a new player and adds it to the player map.
			 * Returns the player's ID, or -1 on error.
			 */
			int addNewPlayer(shared_ptr<Meteorite::Network::Client> client);

			/*
			 * Adds a new message handler.
			 * Syntax of slot: shared_ptr<Meteorite::Game::Player>, shared_ptr<Meteorite::Network::Message>
			 */
			boost::signals2::connection addMessageHandler(uint8_t messageId, const MessageHandler::slot_type& slot);

		private:
			shared_ptr<Meteorite::Application> app;

			std::map<uint8_t, shared_ptr<Meteorite::Game::Player>> players;
			MessageHandler messageHandlers[0x44 + 1]; // 0x44 is highest message ID

			void onConnectRequest(shared_ptr<Meteorite::Game::Player> player, shared_ptr<Meteorite::Network::Message> message);
		};
	}
}