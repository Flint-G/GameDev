#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <enet.h>
#include <string>
#include <vector>
#include <iostream>

class NetworkManager {
public:
    NetworkManager() : host(nullptr), peer(nullptr) {}
    ~NetworkManager() {
        if (host) {
            enet_host_destroy(host);
        }
        enet_deinitialize();
    }

    bool init() {
        if (enet_initialize() != 0) {
            std::cerr << "An error occurred while initializing ENet.\n";
            return false;
        }
        return true;
    }

protected:
    ENetHost* host;
    ENetPeer* peer; // For client: connection to server. For server: unused (peers managed inside host)
};

#endif // NETWORK_MANAGER_H
