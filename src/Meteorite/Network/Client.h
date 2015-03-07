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

#include <boost/array.hpp>

namespace Meteorite
{
	class Application;

	namespace Network
	{
		class Protocol;
		struct Message;

		class Client : public enable_shared_from_this<Meteorite::Network::Client>
		{
		public:
			Client(shared_ptr<Meteorite::Application> app, shared_ptr<Meteorite::Network::Protocol> protocol);
			~Client();

			/*
			 * Sets the client to alive and starts reading data.
			 */
			void start();

			/*
			 * Closes the client's socket and marks it as dead.
			 */
			void stop();

			/*
			 * Begins asynchronously waiting for data.
			 */
			void read();

			/*
			 * Writes any outgoing messages to the socket.
			 */
			void write();

		private:
			/*
			 * e: Boost error code
			 * n: Number of bytes read
			 */
			void handleRead(const boost::system::error_code& e, size_t n);

			/*
			 * e: Boost error code
			 * n: Number of bytes written
			 */
			void handleWrite(const boost::system::error_code& e, size_t n);
			
		public:
			/*
			 * Returns true if the client is still alive.
			 */
			bool isAlive();

			/*
			 * Returns a reference to the client's boost socket.
			 */
			boost::asio::ip::tcp::socket& getSocket();

			/*
			 * Returns a reference to the vector of incoming messages.
			 */
			std::vector<shared_ptr<Meteorite::Network::Message>>& getIncoming();

			/*
			 * Returns a reference to the vector of outgoing messages.
			 */
			std::vector<shared_ptr<Meteorite::Network::Message>>& getOutgoing();

		private:
			shared_ptr<Meteorite::Application> app;
			shared_ptr<Meteorite::Network::Protocol> protocol;

			bool alive = false;
			boost::asio::ip::tcp::socket socket;
			boost::array<uint8_t, 8192> tmp;
			std::vector<uint8_t> incomingBuffer, outgoingBuffer;
			std::vector<shared_ptr<Meteorite::Network::Message>> incoming, outgoing;
		};
	}
}