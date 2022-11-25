#ifndef FILES_H
#define FILES_H

#include <string>
#include <filesystem>
#include <fstream>


// Creates a directory with error checking to see if already exists
// \return True - If directory was already found OR created it now. False - If directory could not be created
bool CreateDirectory(const std::string path);

// Creates a new file (extension needed). Will not create if file already exists
// \return True - Creation successful. False - Could not create file
bool CreateFile(const std::string path);

// \return True - file exists. False - File does not exist
bool DoesFileExist(const std::string path);

// Checks if file exists before writing
// \returns TRUE - Wrote to file successfully  \return FALSE - File could not be found
bool AppendToFile(const std::string path, const std::string& text);


#endif