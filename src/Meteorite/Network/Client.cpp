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

#include <Meteorite/Network/Client.h>

#include <Meteorite/Application.h>
#include <Meteorite/Network/NetworkManager.h>
#include <Meteorite/Network/Message.h>
#include <Meteorite/Network/Protocol.h>

namespace Meteorite
{
	namespace Network
	{
		Client::Client(shared_ptr<Meteorite::Application> app, shared_ptr<Meteorite::Network::Protocol> protocol) :
			app(app), protocol(protocol), socket(app->getNetworkManager()->getService())
		{
		}

		Client::~Client()
		{
		}

		void Client::start()
		{
			alive = true;
			read();
		}

		void Client::stop()
		{
			socket.close();
			alive = false;
		}

		void Client::read()
		{
			socket.async_read_some(
				boost::asio::buffer(tmp),
				boost::bind(&Meteorite::Network::Client::handleRead,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}

		void Client::write()
		{
			if (outgoing.empty())
				return;

			protocol->compose(outgoingBuffer, outgoing);
			outgoing.clear();

			socket.async_write_some(
				boost::asio::buffer(outgoingBuffer),
				boost::bind(&Meteorite::Network::Client::handleWrite,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		}

		void Client::handleRead(const boost::system::error_code& e, size_t n)
		{
			if (!e)
			{
				incomingBuffer.insert(incomingBuffer.end(), tmp.begin(), tmp.begin() + n);

				int state;

				do
				{
					Meteorite::Network::Message* message = nullptr;
					state = protocol->parse(incomingBuffer, &message);

					if (state == Meteorite::Network::Protocol::STATE_GOOD)
						incoming.push_back(shared_ptr<Meteorite::Network::Message>(message));
				} while (state == Meteorite::Network::Protocol::STATE_GOOD);

				read();
			}
			else if (e != boost::asio::error::operation_aborted)
				stop();
		}

		void Client::handleWrite(const boost::system::error_code& e, size_t n)
		{
			if (e)
			{
				outgoingBuffer.clear();
				return;
			}

			outgoingBuffer.erase(outgoingBuffer.begin(), outgoingBuffer.begin() + n);

			if (outgoingBuffer.size() > 0)
				write();
		}

		bool Client::isAlive()
		{
			return alive;
		}

		boost::asio::ip::tcp::socket& Client::getSocket()
		{
			return socket;
		}

		std::vector<shared_ptr<Meteorite::Network::Message>>& Client::getIncoming()
		{
			return incoming;
		}

		std::vector<shared_ptr<Meteorite::Network::Message>>& Client::getOutgoing()
		{
			return outgoing;
		}
	}
}