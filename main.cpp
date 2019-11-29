#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>

int main(int argc, char* argv[]) {
    int pipeFdToServ;
    int pipeFdFromServ;
    
    pipeFdToServ = std::stoi(argv[0]);
    pipeFdFromServ = std::stoi(argv[1]);
    
    std::string outStr;
    int msgSize, inMsg;
    while (true) {
        std::getline(std::cin, outStr);
        int a = outStr.length();
        outStr[a] = '\n';
        outStr[a + 1] = '\0';
        if (write(pipeFdToServ, outStr.c_str(), a + 1) == -1) {
            std::cout << "Cannot write to pipe\n";
            exit(-1);
        }
        read(pipeFdFromServ, &msgSize, sizeof(int));
        std::cout << "Mesage no: " << msgSize << '\n';
        if (msgSize == -2) {
            break;
        }
        for (int i = 0; i < msgSize; i++) {
            read(pipeFdFromServ, &inMsg, sizeof(int));
            std::cout << inMsg << '\n';
        }
    }
    return 0;
}

