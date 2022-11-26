#ifndef RS_TOOLKIT_H
#define RS_TOOLKIT_H

#include <string>
#include <vector>


// Forward Declarations

// --------------------

enum class LogTarget {
    FILE,			// Will show everything BUT runtime logs
    CONSOLE,		// Default. Will show runtime logs
    ALL				// If this is chosen, will log to console and file
};
enum class LogCode {
    FATAL,			// Appliation cannot continue. Exits application with ASSERT after logging
    WARNING,		// Undesirable event, but can still proceed
	ERROR, 			// Error occurred and function cannot continue
   
    // Will be written to Log file
    LOG_LOW,        // Low importance       - Ex: Setting variables. Get an idea of current execution place
    LOG_MED,        // Medium importance    - Ex: To show if something passed/failed an if statement
    LOG_HIGH,       // High importance      - Ex: Function completed, show iteration status


    // Will not be written to Log file
    RUNTIME_LOW,     // Low importance       - Ex: Setting variables. Get an idea of current execution place
    RUNTIME_MED,     // Medium importance    - Ex: To show if something passed/failed an if statement
    RUNTIME_HIGH,    // High importance      - Ex: Function completed, show iteration status

//  RETURN,			// Used in conjunction with function to categorize that msg along with the return

    RST,            // An error happened internal in the Ryan Software Toolkit
};
enum class LogLevel {
    ALL     = 0,    // Shows every LogCode
    FATAL   = 1,    // Shows FATAL LogCodes
    NORMAL  = 2,    // Shows FATAL, WARNING and ERROR LogCodes

//  RETURN  = 2,

    HIGH    = 3,    // Shows FATAL, WARNING, ERROR and HIGH LogCodes
    MED     = 4,    // Shows FATAL, WARNING, ERROR, HIGH and MED LogCodes
    LOW     = 5,    // Shows FATAL, WARNING, ERROR, HIGH. MED and LOW LogCodes
};



namespace RST {

 
    // Initializes the development toolkit. If not given, no diectory will be made.
    // Inside the parentDirectory, a folder labeled "RST Files" will be made
    // \param parentDirectory Folder to store necessary files
    bool init(const std::string parentDirectory = "");


    // Logging --------------------------------------
   
    // Writes a debug log to the specified output target
    bool Log(const std::string msg, const LogCode code);

    // Specifies where to output all Logs. Default is to the console only
    void SetLogTarget(const LogTarget target);

    // Specifies what logs to display. Levels include all of the above levels.
    void SetLogLevel(const LogLevel level);

    // Overwrites parameter with list of all logs in formatted strings
    void GetFormattedLogs(std::vector<std::string>& list);


    // ------------------------------------------------

}


#endif