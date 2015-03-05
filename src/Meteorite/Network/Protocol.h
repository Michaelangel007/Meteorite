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

#include <Meteorite/Network/DataReader.h>
#include <Meteorite/Network/DataWriter.h>

namespace Meteorite
{
	class Application;

	namespace Network
	{
		struct Message;

		/*
		 * CMSG : Client -> Server
		 * SMSG : Server -> Client
		 * MSG  : Both ways
		 */
		enum PacketTypes
		{
			CMSG_CONNECT_REQUEST = 0x01,
		};

		class Protocol
		{
		public:
			enum
			{
				STATE_GOOD,
				STATE_STOP,
				STATE_ERROR
			};

			Protocol(shared_ptr<Meteorite::Application> app);

			int parse(std::vector<uint8_t>& bytes, Meteorite::Network::Message** out);
			int compose(std::vector<uint8_t>& bytes, const std::vector<shared_ptr<Meteorite::Network::Message>>& outgoing);

		private:
			shared_ptr<Meteorite::Application> app;

			Meteorite::Network::DataReader reader;
			Meteorite::Network::DataWriter writer;
		};
	}
}