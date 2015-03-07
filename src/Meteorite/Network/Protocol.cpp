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
#include <Meteorite/Network/Message/All.h>

int GetLengthOfVarint(unsigned int value)
{
	int i = 0;
	do
	{
		uint8_t nextByte = value & 0x7F;
		value >>= 7;

		if (value)
			nextByte |= 0x80;

		i++;
	} while (value);

	return i;
}

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

			reader.setBuffer(&bytes);
			reader.setPos(0);

			Meteorite::Network::Message* message = nullptr;

			int state = Meteorite::Network::Protocol::STATE_ERROR;

			switch (id)
			{
			case CMSG_CONNECT_REQUEST:
			{
				message = new Meteorite::Network::Message_ConnectRequest;
				auto msg = (Meteorite::Network::Message_ConnectRequest*) message;
				reader.read(msg->length);
				reader.read(msg->id);
				reader.read(msg->version);

				if (reader.isValid())
					state = Meteorite::Network::Protocol::STATE_GOOD;

				break;
			}

			default:
			{
				int16_t len;
				reader.read(len);
				reader.skip(len - 2);

				if (reader.isValid())
					state = Meteorite::Network::Protocol::STATE_GOOD;

				char str[256];
				sprintf(str, "Received unhandled packet with header: 0x%02x (%i bytes)", id, len);
				app->getLogger()->info(str);

				break;
			}
			}

			if (state == Meteorite::Network::Protocol::STATE_GOOD)
			{
				*out = message;
				reader.trim();
			}
			else
			{
				delete message;
			}

			reader.reset();

			return state;
		}

		int Protocol::compose(std::vector<uint8_t>& bytes, const std::vector<shared_ptr<Meteorite::Network::Message>>& outgoing)
		{
			writer.setBuffer(&bytes);

			for (auto it = outgoing.begin(); it != outgoing.end(); ++it)
			{
				auto message = (*it).get();

				switch (message->id)
				{
				case SMSG_FATAL_ERROR:
				{
					auto msg = (Meteorite::Network::Message_FatalError*) message;
					msg->length = sizeof(msg->length) + sizeof(msg->id) + GetLengthOfVarint(msg->error.length()) + msg->error.length();

					writer.write(msg->length);
					writer.write(msg->id);
					writer.write(msg->error);
				}
				break;

				case SMSG_CONNECTION_APPROVED:
				{
					auto msg = (Meteorite::Network::Message_ConnectionApproved*) message;
					msg->length = sizeof(msg->length) + sizeof(msg->id) + sizeof(msg->playerSlot);

					writer.write(msg->length);
					writer.write(msg->id);
					writer.write(msg->playerSlot);
				}
				break;
				}
			}

			return Meteorite::Network::Protocol::STATE_GOOD;
		}
	}
}