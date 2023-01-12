#include "RST_internal.h"

#include "Files.h"
#include <iostream>
#include <iomanip>


#pragma warning(disable : 4996)		// Disables warning of using of time_t and tm in creating the Log file name


// Define static variables
std::vector<LogEntry> RST_Application::s_logList{};

// Log Entry
LogEntry::LogEntry(std::string msg, LogCode code)
	: m_msg{msg}, m_code{code}
{

}
std::string LogEntry::str() const{
	std::ostringstream os;
	os << *this;

	return os.str();

}


// RST_Application
RST_Application::RST_Application(std::string parentPath)
{
	if(parentPath != ""){
		SetPaths(parentPath);
	}
}
RST_Application::~RST_Application() {

}
void RST_Application::SetPaths(std::string parentPath) {

	if(!m_init){
		m_DIRECTORY			= parentPath;
		m_LOG_DIRECTORY		= m_DIRECTORY + "Log Files\\";
		m_LOGFILE			= m_LOG_DIRECTORY + CreateLogFileName(m_initTime);

		// Once path variables are set, create them
		CreateDirectory		(m_DIRECTORY);
		CreateDirectory		(m_LOG_DIRECTORY);
		CreateFile			(m_LOGFILE);

		m_init = true;
		
		// Log to the newly created file
		LogTarget buf = APPLICATION.getLogTarget();		// Store current LogTarget
		APPLICATION.SetLogTarget(LogTarget::FILE);
		RST_Application::Log("RST Paths Set. Log file created.", LogCode::RST);
		APPLICATION.SetLogTarget(buf);					// Reset m_logTarget to the buffer

	}
	else{
		RST_Application::Log("RST paths were already set!", LogCode::RST);
	}


	return;
}
bool RST_Application::Log(const std::string msg, const LogCode code) {
	bool success{false};

	LogEntry* buffer = new LogEntry(msg, code);		// Make LogEntry buffer
	s_logList.push_back(*buffer);					// Add to list of known logs;

	// If UserLogVector is defined write logs to it
	if(m_UserLogVector){
		// If the user vector is != one below the logList, write all logs to user vector
		if(m_UserLogVector->size() != s_logList.size() - 1){
			*m_UserLogVector = getFormattedLogs();
		}
		// If user vector is one below (it should be since we just added one more log before this if statement), add the new log to user vector
		else{
			m_UserLogVector->push_back(buffer->str());
		}
	}


	// Filter LogEntry to see if it should be outputted based on current LogTarget and LogLevel
	if(FilterLog(buffer->m_code)){

		LogTarget* logTarget = new LogTarget(APPLICATION.getLogTarget()); // Buffer to hold current LogTarget state

		// If so far the LogLevel and parameter LogCode allows, see if its a RUNTIME LogCode
		switch(code){
			case 	LogCode::RUNTIME_HIGH: 
			case 	LogCode::RUNTIME_MED: 
			case 	LogCode::RUNTIME_LOW: 

				if(*logTarget == LogTarget::CONSOLE){
					LogToConsole(*buffer);			
					success = true;
				}
				else if (*logTarget == LogTarget::ALL){
					LogToConsole(*buffer); 			// Log just to console
				}
				
			break;
			// Everything but the RUNTIME Logs
			case LogCode::FATAL:
			case LogCode::WARNING:
			case LogCode::ERROR:
			case LogCode::LOG_LOW:
			case LogCode::LOG_MED:
			case LogCode::LOG_HIGH:
			case LogCode::RST:

				// Output LogEntry to where its needed
				switch(*logTarget){
					case LogTarget::ALL:
						LogToConsole(*buffer);
						success = LogToFile(*buffer);
					break;
					case LogTarget::CONSOLE:
						LogToConsole(*buffer);			
						success = true;
					break;
					case LogTarget::FILE:
						success = LogToFile(*buffer);
					break;
					default:
						RST_ASSERT("RST_Application::Log() - could not find LogTarget");
						success = false;
					break;
				}
			break;
			default:
				RST_ASSERT("RST_Application::Log() - could not find LogCode to decide where to output");
				success = false;
			break;
		}

		delete logTarget;

		// If LogCode is fatal, write message and exit application:
		if(code == LogCode::FATAL){
			FATAL_ASSERT(buffer->m_msg);
		}

	}
	else{
		// Do Nothing
	}

	delete buffer;


	
	return success;
}
bool RST_Application::LogToFile(const LogEntry& log){
	bool success{false};

	// If files were not initialized write to the console and exit function
	if(!m_init){

		// All for removing the last \n character
		std::string buffer;
		std::ostringstream os; os << log; buffer = os.str();
		if(buffer.back() == '\n'){
			buffer.pop_back();
		}

		RST_Log("RST Files were not initialized. Meant for file:  " + buffer);
		RST_ASSERT("RST was not initialized.");
		return false;
	}

	success = AppendToFile(APPLICATION.getLogFile(), log.str());
	

	// If could not write to file, say why in the console
	if(!success){
		RST_Log("Log File [" + APPLICATION.getLogFile() + "] does not exist");
	}

	return success;
}
void RST_Application::LogToConsole(const LogEntry& log){
	if(s_logList.empty()){
		return;
	}
	else{
		std::ostringstream text;
		text << log;
		std::cout << text.str();
	}

	return;
}
const std::vector<std::string> RST_Application::getFormattedLogs() const {

	std::vector<std::string> buffer{};

	for(LogEntry log : s_logList){
		std::ostringstream os;

		os << log;
		buffer.push_back(os.str());
	}

	return buffer;
}

// Helper functions
std::string CreateLogFileName(time_t exactTIme) {
	tm* ltm = localtime(&exactTIme);

	std::ostringstream name;

	name << std::setw(2) << std::setfill('0') << ltm->tm_mday << '-';
	switch (ltm->tm_mon) {
	case 0:
		name << "JAN";
		break;
	case 1:
		name << "FEB";
		break;
	case 2:
		name << "MAR";
		break;
	case 3:
		name << "APR";
		break;
	case 4:
		name << "MAY";
		break;
	case 5:
		name << "JUN";
		break;
	case 6:
		name << "JUL";
		break;
	case 7:
		name << "AUG";
		break;
	case 8:
		name << "SEP";
		break;
	case 9:
		name << "OCT";
		break;
	case 10:
		name << "NOV";
		break;
	case 11:
		name << "DEC";
		break;
	default:
		name << "MMM";
	}


	int year{ ltm->tm_year + 1900 };

	name << '-' << year;

	// THIS DISPLAYS THE TIME IN HH-MM-SS format

	name << "_"
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << '-'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << '-'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec;

	name << ".dat";

	return name.str();
}
std::ostream& operator<<(std::ostream& os, const LogEntry& log) {

	tm* ltm = localtime(&log.m_timestamp);

	//Time in HH:MM:SS format
	os << "["
		<< std::setw(2) << std::setfill('0') << ltm->tm_hour << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_min << ':'
		<< std::setw(2) << std::setfill('0') << ltm->tm_sec << "]";

	os << "  [" << std::setw(7) << std::setfill(' ');
	switch (log.m_code) {
	case LogCode::FATAL:
		os << "FATAL ";
		break;
	case LogCode::WARNING:
		os << "WARNING";
		break;
	case LogCode::ERROR:
		os << "ERROR ";
		break;

	// Log
	case LogCode::LOG_LOW:
		os << "LOG LOW";
		break;
	case LogCode::LOG_MED:
		os << "LOG MED";
		break;
	case LogCode::LOG_HIGH:
		os << "LOG HIGH";
		break;
	
	// Runtime
	case LogCode::RUNTIME_LOW:
		os << "RUN LOW";
		break;
	case LogCode::RUNTIME_MED:
		os << "RUN MED";
		break;
	case LogCode::RUNTIME_HIGH:
		os << "RUN HIGH";
		break;

	
/*	case LogCode::RETURN:
		os << "RETURN";
		break;
*/
	case LogCode::RST:
		os << "  RST  ";
		break;
	default:
		os << "LOG CODE OUTPUT NOT FOUND";
		break;
	}
	os << "]";

	os << "\t" << log.m_msg << '\n';

	return os;
}
void RST_Log(std::string msg){
	LogTarget current {APPLICATION.getLogTarget()};			// Store current LogTarget
	APPLICATION.SetLogTarget(LogTarget::CONSOLE);			// Set LogTarget to console
	APPLICATION.RST_Application::Log(msg, LogCode::RST); 	// Log what happened using RST LogCode
	APPLICATION.SetLogTarget(current);						// Return LogTarget to prior

	return;
}
bool FilterLog(const LogCode code){
	bool success{false};

	// No matter what if the LogCode is RST, it will display
	if(code == LogCode::RST){
		return true;
	}

	// B
	switch(APPLICATION.getLogLevel()){
		case LogLevel::ALL:					// Shows every log
			success = true;
		break;
		case LogLevel::FATAL:				// Only shows Fatal logs
			if(code == LogCode::FATAL) 	{ success = true; 	}
			else						{ success = false; 	}
		break;
		case LogLevel::NORMAL:				// Shows FATAL, WARNING, ERROR
			if(code == LogCode::FATAL ||  code == LogCode::WARNING || code == LogCode::ERROR ) 	
					{ success = true; 	}
			else	{ success = false; 	}
		break;
		case LogLevel::HIGH:				// Shows FATAL, WARNING, ERROR, LOG/RUN HIGH
			if(    code == LogCode::FATAL 		||  code == LogCode::WARNING 		|| code == LogCode::ERROR 
				|| code == LogCode::LOG_HIGH 	|| code == LogCode::RUNTIME_HIGH ) 	
					{ success = true; 	}
			else	{ success = false; 	}
		break;
		case LogLevel::MED:					// Shows FATAL, WARNING, ERROR, LOG/RUN HIGH & MED
			if(	   code == LogCode::FATAL 		||  code == LogCode::WARNING 		|| code == LogCode::ERROR 
				|| code == LogCode::LOG_HIGH 	|| code == LogCode::RUNTIME_HIGH 
				|| code == LogCode::LOG_MED 	|| code == LogCode::RUNTIME_MED	) 	
					{ success = true; 	}
			else	{ success = false; 	}
		break;
		case LogLevel::LOW:					// Shows FATAL, WARNING, ERROR, LOG/RUN HIGH, MED, & LOW
			if(code == LogCode::FATAL ||  code == LogCode::WARNING || code == LogCode::ERROR 
				|| code == LogCode::LOG_HIGH 	|| code == LogCode::RUNTIME_HIGH
				|| code == LogCode::LOG_MED 	|| code == LogCode::RUNTIME_MED
				|| code == LogCode::LOG_LOW 	|| code == LogCode::RUNTIME_LOW) 	
					{ success = true; 	}
			else	{ success = false; 	}
		break;
		default:
			RST_ASSERT("FilterLogLevel() - no LogLevel was found");
		break;
	}



	return success;
}



// RST namespace
// Definitions of public API funcions

namespace RST {

	bool init(std::string parentDirectory) {
		bool success{ false };	// Returned variable

		const std::string RST_DIRECTORY_NAME { "RST Files\\" }; // Must have backslash

		// If RST was already initialized, exit
		if(APPLICATION.m_init) {
			RST_Log("Application already initialized");
			RST_ASSERT("RST::init() called twice. RST was already initialized.")
			success = false;
		}

		// If RST has not been initialized, init files
		else {
			if (parentDirectory == "") {
				success = false;
				RST_Log("No parent directory was given");
			}
			else {
				// Ensure there is a backslash at the end
				if (parentDirectory.back() != '\\' && parentDirectory.back() != '/') {
					parentDirectory += "\\";
				}

				parentDirectory += RST_DIRECTORY_NAME; // Give parent directory the extension to store RST Files

				// Create the directory to initialize RST_Application
				if (!CreateDirectory(parentDirectory)) {
					RST_Log("Could not create directory " + parentDirectory + "");
					success = false;
				}

				// Directory was created
				else {
					APPLICATION.SetPaths(parentDirectory);
					success = true;
				}
			}
		}

		return success;
	}

    bool Log(const std::string msg, const LogCode code){
		return APPLICATION.RST_Application::Log(msg, code);
	}

	void SetLogTarget(const LogTarget target){
		APPLICATION.SetLogTarget(target);
		return;
	}
	void SetLogLevel(const LogLevel level){
		APPLICATION.SetLogLevel(level);
		return;
	}
	void SetLogVector(std::vector<std::string>* vector){
		static bool alreadyCalled { false };

		if(!APPLICATION.getLogVector()){
			alreadyCalled = false;
		}

		if(!alreadyCalled){
			APPLICATION.SetUserLogVector(vector);
			RST::Log("User has defined a vector to store logs", LogCode::RST);
			alreadyCalled = true;
		}
		else{
			RST::Log("Log Vector has already been initialized and not yet deleted using RST::DeleteLogVector(). Cannot redefine. Stopping execution.", LogCode::RST);
			RST_ASSERT("Log Vector has already been initialized and not yet deleted using RST::DeleteLogVector(). Did you call this function twice?");
		}
		
		return;
	}
	void DeleteLogVector(){
		APPLICATION.SetUserLogVector(nullptr);
		RST::Log("User defined Log vector was set to nullptr", LogCode::RST);
	}
}
// ---

