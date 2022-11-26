#ifndef RST_INTERNAL_H
#define RST_INTERNAL_H

#define ASSERT(msg) \
std::cerr << "\n\nASSERTION failed in " << __FILE__ << " line " << __LINE__ << ": " << msg << std::endl; \
std::terminate();

#include "RST.h"

#include <vector>
#include <sstream>


class LogEntry{
public:
	LogEntry(std::string msg, LogCode code);
	~LogEntry() {};

    const std::string 	m_msg;
	const LogCode 		m_code;

	std::string str() const;

	friend std::ostream& operator<<(std::ostream& os, const LogEntry& log);

private:

	const time_t m_timestamp{ time(0) };
};


class RST_Application {
public:

	RST_Application(std::string parentPath="");
	~RST_Application();

	// Getter functions
	inline const std::string& 	getDirectory()			const { return m_DIRECTORY;		}
	inline const std::string& 	getLogDirectory()		const { return m_LOG_DIRECTORY; }
	inline const std::string& 	getLogFile()			const { return m_LOGFILE;		}
	inline const LogLevel& 	  	getLogLevel()			const { return m_logLevel;		}
	inline const LogTarget& 	getLogTarget()			const { return m_logTarget;		}

	const void getFormattedLogs(std::vector<std::string>& list)	const;


	// Setter functions
	inline void SetLogTarget	(const LogTarget target) 	{ m_logTarget = target; }
	inline void SetLogLevel		(const LogLevel	 level)		{ m_logLevel = level;   }

	// Adds to List of Logs
	bool Log(const std::string msg, const LogCode code);


	friend bool RST::init(std::string parentDirectory);

private:
	static std::vector<LogEntry> s_logList;		// List of all logs in the instance

	bool m_init {false};
	std::string m_DIRECTORY;
	std::string m_LOG_DIRECTORY;
	std::string m_LOGFILE;


	LogTarget	m_logTarget	{ LogTarget::CONSOLE };
	LogLevel	m_logLevel	{ LogLevel::NORMAL };

	const time_t m_initTime{ time(0) };

	void SetPaths(std::string parentPath);	// Initialize Parent Paths
	bool LogToFile		(const LogEntry& log);	// Outputs to file
	void LogToConsole	(const LogEntry& log);	// Outputs to console
};	


RST_Application APPLICATION{};	 // Global State Variable for use everywhere

// Helper Functions

std::string CreateLogFileName(time_t exactTIme);	// Returns string in format 12-DEC-2022_13:14:09 (Hour:Min:Sec)
void RST_Log(std::string msg);

// If App's m_logLevel allows the buffer's m_logCode to be written, return true
bool FilterLog(const LogCode code);


#endif
