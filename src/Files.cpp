#include "Files.h"

bool CreateDirectory(const std::string path) {
	bool success{ false };

	// Before creating, check if it already exists. If not found, create it
	if (DoesFileExist(path)) { success = true; }
	else {

		// If the directory could not be created return false
		if (!std::filesystem::create_directory(path))
		{ 
			success = false;
		}	
		success = true;
	}
	return success;
}


bool CreateFile(const std::string path) {
	if (!DoesFileExist(path)) {
		std::ofstream file(path);	//Creates the files here

		// If the file still does not exist, return false	
		if (!DoesFileExist(path)) 	{ return false; }
		else 						{ return true; }
	}
	else { return true; }
}


bool DoesFileExist(const std::string path) {
	const std::filesystem::path checkFile(path);

	if (std::filesystem::exists(checkFile))		{ return true; }
	else 										{ return false; }
}


bool AppendToFile(const std::string path, const std::string& text){
    //If the file doesnt exist, returns false
    if (!DoesFileExist(path)) {
       return false;
    }
    else {
        std::ofstream file{ path, std::ios_base::app}; //Open file to write
        file << text;
        return true;
    }
	
    return false;
}