#include <iostream>
#include <vector>

#include <RST.h>


int main(){

    RST::init("C:\\dev\\1. Project Development Files");

    RST::SetLogLevel(LogLevel::ALL);

    RST::Log("Fatal", LogCode::FATAL);
    RST::Log("Warning", LogCode::WARNING);
    RST::Log("Error", LogCode::ERROR);

    RST::Log("Run High", LogCode::RUNTIME_HIGH);
    RST::Log("Log High", LogCode::LOG_HIGH);
    RST::Log("Log Med", LogCode::LOG_MED);

    std::vector<std::string>* myLogs = new std::vector<std::string>;

    RST::SetLogVector(myLogs);


    RST::Log("Run High 2", LogCode::RUNTIME_HIGH);
    RST::Log("Log High 2", LogCode::LOG_HIGH);
    RST::Log("Log Med 2", LogCode::LOG_MED);

    std::cout << "\n\nOutput all logs:\n";
    for(std::string s : *myLogs){
        std::cout << s;
    }

    RST::DeleteLogVector();
    
    
    std::cout << "\n\nNew section:\n";
    RST::Log("Run High 3", LogCode::RUNTIME_HIGH);
    RST::Log("Log High 3", LogCode::LOG_HIGH);
    RST::Log("Log Med 3", LogCode::LOG_MED);


    RST::SetLogVector(myLogs);
    std::cout << "\n\nOutput all logs:\n";
    for(std::string s : *myLogs){
        std::cout << s;
    }

    return 0;
}