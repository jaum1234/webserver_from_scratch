#include "TcpServer.hpp"
#include "Logger.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <sstream>

using namespace http;
using namespace logging;

const int BUFFER_SIZE = 30720;

TcpServer::TcpServer(
	int domain, 
	int type, 
	int protocol, 
	string ip_address, 
	int port
) : 
	m_ip_address(ip_address), 
	m_port(port),
	m_socketAddress(),
	m_sock(),
	m_new_socket(),
	m_socketAddressLen(sizeof(m_socketAddress))
{
	m_socketAddress.sin_family = domain;
	m_socketAddress.sin_port = htons(port);
	m_socketAddress.sin_addr.s_addr = inet_addr(ip_address.c_str()); 

	logger = new Logger();
	TcpServer::start(domain, type, protocol);
}

int TcpServer::start(int domain, int type, int protocol) {
	logger->info("Starting server.");

	logger->info("Creating socket.");
	m_sock = socket(domain, type, protocol);

	if (m_sock < 0) {
		logger->error("Cannot create socket.");
		exit(1);
		return 1;
	}

	logger->info("Socket created.");

	logger->info("Binding socket.");
	int m_bind = bind(m_sock, (sockaddr*) &m_socketAddress, m_socketAddressLen);

	if (m_bind < 0) {
		logger->error("Cannot bind socket.");
		close(m_sock);	
		exit(1);
	}

	logger->info("Socket binded.");

	TcpServer::listenConnections(20);

	return 0;
}

void TcpServer::listenConnections(int maxConnThreads) {
	if (listen(m_sock, maxConnThreads) < 0) {
		logger->error("Socket listen failed.");
		exit(1);
	}

	ostringstream message;

	message << "Listening on PORT : " << ntohs(m_socketAddress.sin_port);

	logger->info(message.str());

	while (true) {
		TcpServer::acceptConnection(m_new_socket);

		TcpServer::readRequest();
		TcpServer::sendResponse();

		close(m_new_socket);
	}	
}

void TcpServer::acceptConnection(int &temp_socket) {
	temp_socket = accept(m_sock, (sockaddr*) &m_socketAddress, &m_socketAddressLen);

	if (temp_socket < 0) {
		ostringstream message;
		message << "Server failed to accept incoming connection from PORT "
			<< ntohs(m_socketAddress.sin_port);
		
		logger->error(message.str());
		exit(1);
	}
}

void TcpServer::readRequest() {
	char buffer[BUFFER_SIZE] = {0};

	int bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);

	if (bytesReceived < 0) {
		logger->error("Failed to read bytes from client socket connection.");
		exit(1);
	}
}

void TcpServer::sendResponse() {
	long bytesSent;
	
	string html = "<!DOCTYPE html><html lang=\"en\"><body><h1>ola mundo</h1></body></html>";
	ostringstream content;

	content << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size()
		<< "\n\n"
		<< html;

	bytesSent = write(m_new_socket, content.str().c_str(), content.str().size());
	
	if (bytesSent != content.str().size()) {
		logger->error("Error sending response to client.");
		exit(1);
	}

	logger->info("Server response sent to client.");
}

void TcpServer::closeServer() {
	logger->info("Closing server.");
	close(m_sock);
	close(m_new_socket);
	logger->info("Server closed.");
	exit(0);
}

TcpServer::~TcpServer() {
	TcpServer::closeServer();
}

