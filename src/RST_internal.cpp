#include "RST_internal.h"


#pragma warning(disable : 4996)		// Disables warning of using of time_t and tm in creating the Log file name


// Define static variables
std::vector<LogEntry> RST_Application::s_logList{};

// Log Entry
LogEntry::LogEntry(std::string msg, LogCode code)
	: m_msg{msg}, m_code{code}
{

}
std::string LogEntry::str(){
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
		AddLog("RST Paths Set", LogCode::RST);
	}
	else{
		AddLog("RST paths were already set!", LogCode::RST);
	}


	return;
}
bool RST_Application::AddLog(std::string msg, LogCode code) {
	bool success{false};

	// Add to list of known logs;
	s_logList.push_back(LogEntry(msg, code));


	// Output where its needed
	switch(APPLICATION.getLogTarget()){
		case LogTarget::ALL:
			LogToConsole();
			success = LogToFile();
		break;
		case LogTarget::CONSOLE:
			LogToConsole();			
			success = true;
		break;
		case LogTarget::FILE:
			success = LogToFile();
		break;
		default:
			success = false;
		break;
	}

	
	return success;
}
bool RST_Application::LogToFile(){
	bool success{false};

	// If files were not initialized write to the console and exit function
	if(!m_init){

		// All for removing the last \n character
		std::string buffer;
		std::ostringstream os; os << s_logList.back(); buffer = os.str();
		if(buffer.back() == '\n'){
			buffer.pop_back();
		}

		RST_Log("RST Files were not initialized. Meant for file:  " + buffer);
		ASSERT("RST was not initialized.");
		return false;
	}

	// If the list is empty, no need to write
	if(s_logList.empty()){
		success = false;
	}
	else{
		success = AppendToFile(APPLICATION.getLogFile(), s_logList.back().str());
	}

	// If could not write to file, say why in the console
	if(!success){
		RST_Log("Log File [" + APPLICATION.getLogFile() + "] does not exist");
	}

	return success;
}
void RST_Application::LogToConsole(){
	if(s_logList.empty()){
		return;
	}
	else{
		std::ostringstream text;
		text << s_logList.back();
		std::cout << text.str();
	}

	return;
}
const void RST_Application::getFormattedLogs(std::vector<std::string>& list) const {
	for(LogEntry log : s_logList){
		std::ostringstream os;

		os << log;
		list.push_back(os.str());
	}

	return;
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

	os << "\t[";
	switch (log.m_code) {
	case LogCode::FATAL:
		os << "FATAL";
		break;
	case LogCode::WARNING:
		os << "WARNING";
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
		os << "RUNTIME LOW";
		break;
	case LogCode::RUNTIME_MED:
		os << "RUNTIME MED";
		break;
	case LogCode::RUNTIME_HIGH:
		os << "RUNTIME HIGH";
		break;

	
	case LogCode::RETURN:
		os << "RETURN";
		break;
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
	LogTarget current {APPLICATION.getLogTarget()};		// Get current target
	APPLICATION.SetLogTarget(LogTarget::CONSOLE);		// Write to console
	APPLICATION.AddLog(msg, LogCode::RST); 				// Log what happened
	APPLICATION.SetLogTarget(current);					// Return target to prior

	return;
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

    bool Log(std::string msg, LogCode code){
		return APPLICATION.AddLog(msg, code);
	}

	void SetLogTarget(const LogTarget target){
		APPLICATION.SetLogTarget(target);
		return;
	}
	void SetLogLevel(const LogLevel level){
		APPLICATION.SetLogLevel(level);
		return;
	}
	void GetFormattedLogs(std::vector<std::string>& list){
		return APPLICATION.getFormattedLogs(list);
	}
}
// ---

