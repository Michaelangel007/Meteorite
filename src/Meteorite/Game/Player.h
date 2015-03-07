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
		class Player
		{
		public:
			Player(shared_ptr<Meteorite::Application> app, shared_ptr<Meteorite::Network::Client> client, uint8_t playerSlot);
			~Player();

			void run();

			shared_ptr<Meteorite::Network::Client> getClient();

			uint8_t getPlayerSlot();

		private:
			shared_ptr<Meteorite::Application> app;
			shared_ptr<Meteorite::Network::Client> client;
			uint8_t playerSlot;
		};
	}
}