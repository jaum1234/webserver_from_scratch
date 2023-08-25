#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <sys/socket.h>
#include "Logger.hpp"

using namespace logging;

namespace http {
	struct in_addr {
		unsigned long s_addr;
	};

	struct sockaddr_in {
		short sin_family;
		unsigned short sin_port;
		struct in_addr sin_addr;
		char sin_zero[8];
	};

	class TcpServer {
		private:
			Logger* logger;
			int m_sock;
			string m_ip_address;
			int m_port;
			struct sockaddr_in m_socketAddress;
			int m_new_socket;
			unsigned int m_socketAddressLen;

			int start(int domain, int type, int protocol);
			void sendResponse();
			void readRequest();
			void closeServer();

		public:
			TcpServer(int domain, int type, int protocol, string address, int port);
			~TcpServer();
			void listenConnections(int maxConnThreads);
			void acceptConnection(int &temp_socket);
	};
}

#endif
