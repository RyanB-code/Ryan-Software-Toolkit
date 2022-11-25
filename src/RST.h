#ifndef RS_TOOLKIT_H
#define RS_TOOLKIT_H

#include <string>
#include <vector>


// Forward Declarations

// --------------------

// enums --------------------------------------------

enum class LogCode {
    FATAL,			// Appliation cannot continue. Exits application execution after logging
    WARNING,		// Undesirable event, but can still proceed
   
    // Will be written to Log file
    LOG_LOW,        // Low importance       - Ex: Setting variables. Get an idea of current execution place
    LOG_MED,        // Medium importance    - Ex: To show if something passed/failed an if statement
    LOG_HIGH,       // High importance      - Ex: Function completed, show iteration status


    // Will not be written to Logfile
    RUNTIME_LOW,        // Low importance       - Ex: Setting variables. Get an idea of current execution place
    RUNTIME_MED,        // Medium importance    - Ex: To show if something passed/failed an if statement
    RUNTIME_HIGH,       // High importance      - Ex: Function completed, show iteration status

    RETURN,			// Used in conjunction with function to categorize that msg along with the return

    RST,            // An error happened internal in the Ryan Software Toolkit
};

enum class LogTarget {
    FILE,			// Will show everything BUT runtime logs
    CONSOLE,		// Default is console only, will show runtime logs
    ALL				// If this is chosen, will show to all
};

// Choose which logs are displayed. Higher levels display all lower levels
// Range 0 to 4
enum LogLevel {
    ALL     = 0,

// Uses this later
/*
    FATAL   = 1,
    RETURN  = 2,
    LOG     = 3,
    RUNTIME = 4,
*/
};

// --------------------------------------------------


namespace RST {

 
    // Initializes the development toolkit. If not given, no diectory will be made.
    // Inside the parentDirectory, a folder labeled "RST Files" will be made
    // \param parentDirectory Folder to store necessary files
    bool init(const std::string parentDirectory = "");


    // Logging --------------------------------------
   
    // Writes a debug log to the specified output target
    bool Log(std::string msg, LogCode code);

    // Specifies where to output all Logs. Default is to the console only
    void SetLogTarget(const LogTarget target=LogTarget::CONSOLE);

    // Specifies what logs to display. Levels include all of the above levels.
    void SetLogLevel(const LogLevel level);

    // Overwrites parameter with list of all logs in formatted strings
    void GetFormattedLogs(std::vector<std::string>& list);


    // ------------------------------------------------

}


#endif