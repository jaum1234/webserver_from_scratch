#include "Logger.hpp"

using namespace logging;

void Logger::info(string message) {
	cout << "INFO: " << message << endl;	
}

void Logger::error(string message) {
	cout << "ERROR: " << message << endl;
}
