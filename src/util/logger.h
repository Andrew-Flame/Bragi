#ifndef BRAGI_LOGGER_H
#define BRAGI_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

enum LogLevel : char {	Info, Warn, Debug, Fatal };

/* Class for logging important messages */
class Logger {
public:
	/* Init the logger (for filesystem) */
	static void Init();
	
	/* Log messages */
	template<typename T>
	static void Info(T message) { Logger::Log(message, LogLevel::Info); }

	template<typename T>
	static void Warn(T message) { Logger::Log(message, LogLevel::Warn); }

	template<typename T>
	static void Debug(T message) { Logger::Log(message, LogLevel::Debug); }

	template<typename T>
	static void Fatal(T message) { Logger::Log(message, LogLevel::Fatal); }
private:
	inline static std::ofstream* stream = nullptr;  //Log file stream
	
	/* Master log method */
	template<typename T>
	static void Log(T message, LogLevel logLevel) {
		std::string color_code, log_level;
		
		switch (logLevel) {
			case LogLevel::Info:
				color_code = "\033[34m";
				log_level = " Info]:  ";
				break;

			case LogLevel::Warn:
				color_code = "\033[33m";
				log_level = " Warn]:  ";
				break;

			case LogLevel::Debug:
				color_code = "\033[35m";
				log_level = " Debug]: ";
				break;

			case LogLevel::Fatal:
				color_code = "\033[31m";
				log_level = " Fatal]: ";
				break;
		}
		
		
		std::string data = '[' + Logger::Date() + log_level;
		std::cout << color_code << data << message << "\033[0m" << std::endl;
		*stream << data << message << std::endl;
	}
	
	/* Clean the old log files */
	static void CleanLogs();
	
	/* Get the current formatted date */
	static std::string Date();
	
	/* Format units, which can starts with zero */
	static std::string UnitFormat(int unit);
};

#endif