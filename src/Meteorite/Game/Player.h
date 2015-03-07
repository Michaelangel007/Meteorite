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
	}

	namespace Game
	{
		enum PlayerState
		{
			STATE_DEFAULT,
			STATE_LOGGING_IN,
			STATE_PLAYING,
		};

		class Player
		{
		public:
			Player(shared_ptr<Meteorite::Application> app, shared_ptr<Meteorite::Network::Client> client, uint8_t playerSlot);
			~Player();

			void run();

			/*
			 * Sends a SMSG_FATAL_ERROR message to the player.
			 */
			void kick(const std::string& reason);

			/*
			 * Returns the network client associated with this player.
			 */
			shared_ptr<Meteorite::Network::Client> getClient();

			/*
			 * Returns the ID of the player (between 0 and 254).
			 */
			uint8_t getPlayerSlot();

			/*
			 * Sets the current state of the player. Certain packets will only be handled
			 * under specific states.
			 */
			void setState(PlayerState state);

			/*
			 * Returns the current state of the player.
			 */
			PlayerState getState();

		private:
			shared_ptr<Meteorite::Application> app;
			shared_ptr<Meteorite::Network::Client> client;
			uint8_t playerSlot;

			PlayerState state = PlayerState::STATE_DEFAULT;
		};
	}
}