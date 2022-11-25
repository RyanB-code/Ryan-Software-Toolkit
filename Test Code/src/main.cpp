#include <iostream>
#include <vector>

#include <RST.h>


int main(){

    RST::SetLogTarget(LogTarget::FILE);
    RST::init("C:\\dev\\1. Project Development Files");
    //RST::init();

    RST::Log("Test MSG", LogCode::LOG_HIGH);
    RST::Log("Test MSG 2", LogCode::LOG_LOW);
    RST::Log("Test MSG 3", LogCode::LOG_MED);

    
    RST::Log("Test MSG 4", LogCode::LOG_LOW);
    RST::Log("Test MSG 5", LogCode::LOG_MED);


    return 0;
}