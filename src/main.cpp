#include "../include/Engine.h"

#include "../include/Server.h"
#include <cstring>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--server") == 0) {
        Server server;
        if (!server.start()) return -1;
        
        std::cout << "Server running... Press Ctrl+C to stop.\n";
        while (true) {
            server.update();
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    } else {
        Engine engine;
        if (!engine.init()) return -1;

        engine.run();
        engine.shutdown();
    }

    return 0;
}
 