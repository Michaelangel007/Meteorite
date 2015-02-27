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

#include <Meteorite/Network/Protocol.h>

#include <Meteorite/Application.h>
#include <Meteorite/Network/Message.h>

#include <Meteorite/Network/Message/ConnectRequest.h>

namespace Meteorite
{
	namespace Network
	{
		Protocol::Protocol(shared_ptr<Meteorite::Application> app) :
			app(app)
		{
		}

		int Protocol::parse(std::vector<uint8_t>& bytes, Meteorite::Network::Message** out)
		{
			if (bytes.size() < 1)
				return Meteorite::Network::Protocol::STATE_STOP;

			uint8_t id = bytes[sizeof(int16_t)];

			ds.setBuffer(&bytes);
			ds.setPos(0);
			ds.setValid(true);


			Meteorite::Network::Message* message = nullptr;

			int state = Meteorite::Network::Protocol::STATE_ERROR;

			switch (id)
			{
			case CMSG_CONNECT_REQUEST:
			{
				message = new Meteorite::Network::Message_ConnectRequest;
				auto msg = (Meteorite::Network::Message_ConnectRequest*) message;
				ds.read(msg->length);
				ds.read(msg->id);
				ds.read(msg->version);

				if (ds.isValid())
					state = Meteorite::Network::Protocol::STATE_GOOD;

				break;
			}

			default:
			{
				int16_t len;
				ds.read(len);
				ds.skip(len - 1);

				if (ds.isValid())
					state = Meteorite::Network::Protocol::STATE_GOOD;

				char str[256];
				sprintf(str, "Received unhandled packet with header: 0x%02x (%i bytes)", id, len);
				app->getLogger()->info(str);

				break;
			}
			}

			if (state = Meteorite::Network::Protocol::STATE_GOOD)
			{
				*out = message;
				ds.remove();
			}
			else
			{
				delete message;
			}

			ds.getBuffer()->clear();
			ds.reset();

			return state;
		}

		int Protocol::compose(std::vector<uint8_t>& bytes, const Meteorite::Network::Message& msg)
		{
			return Meteorite::Network::Protocol::STATE_GOOD;
		}
	}
}