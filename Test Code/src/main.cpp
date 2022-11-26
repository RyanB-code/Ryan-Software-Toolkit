#include <iostream>
#include <vector>

#include <RST.h>


int main(){

    RST::init("C:\\dev\\1. Project Development Files");

    RST::SetLogTarget(LogTarget::CONSOLE);
    RST::SetLogLevel(LogLevel::ALL);

    RST::Log("Fatal", LogCode::FATAL);
    RST::Log("Warning", LogCode::WARNING);
    RST::Log("Error", LogCode::ERROR);

    RST::Log("Run High", LogCode::RUNTIME_HIGH);
    RST::Log("Log High", LogCode::LOG_HIGH);
    RST::Log("Log Med", LogCode::LOG_MED);



    return 0;
}