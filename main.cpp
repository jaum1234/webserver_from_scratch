#include "TcpServer.hpp"

using namespace http;

int main() {
	TcpServer server(AF_INET, SOCK_STREAM, 0, "0.0.0.0", 8080);

	return 0;
}
