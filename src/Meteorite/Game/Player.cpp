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

#include <Meteorite/Game/Player.h>

#include <Meteorite/Application.h>

namespace Meteorite
{
	namespace Game
	{
		Player::Player(shared_ptr<Meteorite::Application> app, shared_ptr<Meteorite::Network::Client> client, uint8_t playerSlot) :
			app(app), client(client), playerSlot(playerSlot)
		{
		}

		Player::~Player()
		{
		}

		void Player::run()
		{
		}

		shared_ptr<Meteorite::Network::Client> Player::getClient()
		{
			return client;
		}

		uint8_t Player::getPlayerSlot()
		{
			return playerSlot;
		}
	}
}