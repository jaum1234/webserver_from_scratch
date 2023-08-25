#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

using namespace std;

namespace logging {
	class Logger {
		public:
			void info(string message);
			void error(string message);
	};
}

#endif 
