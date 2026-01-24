#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkManager.h"
#include "Packet.h"
#include <map>

class Client : public NetworkManager {
public:
    struct RemotePlayer {
        glm::vec2 currentPos;
        std::string name;
    };

    uint32_t myId = 0;
    std::map<uint32_t, RemotePlayer> otherPlayers; // ID -> RemotePlayer
    bool hasSpawned = false;
    glm::vec2 myStartPosition;

    Client() {}

    bool connect(const std::string& hostName) {
        if (!init()) return false;

        host = enet_host_create(NULL, 1, 2, 0, 0);
        if (host == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet client host.\n";
            return false;
        }

        ENetAddress address;
        enet_address_set_host(&address, hostName.c_str());
        address.port = SERVER_PORT;

        peer = enet_host_connect(host, &address, 2, 0);
        if (peer == nullptr) {
            std::cerr << "No available peers for initiating an ENet connection.\n";
            return false;
        }

        // Wait for connection
        ENetEvent event;
        if (enet_host_service(host, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "Connection to " << hostName << " succeeded.\n";
            
            // Send Join Request
            PacketJoinRequest req;
            req.header.type = PacketType::JOIN_REQUEST;
            req.header.playerId = 0; 
            sendPacket(&req, sizeof(req));

        } else {
            enet_peer_reset(peer);
            std::cout << "Connection to " << hostName << " failed.\n";
            return false;
        }
        return true;
    }

    void update(float deltaTime = 0.0f) {
        ENetEvent event;
        while (enet_host_service(host, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    handlePacket((PacketHeader*)event.packet->data);
                    enet_packet_destroy(event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server.\n";
                    break;
                default: break;
            }
        }
    }

    void sendPosition(const glm::vec2& pos) {
        if (myId == 0) return; // Not joined yet

        PacketPlayerUpdate pkt;
        pkt.header.type = PacketType::PLAYER_UPDATE;
        pkt.header.playerId = myId;
        pkt.position = pos;
        
        // Send unreliable for movement
        ENetPacket* packet = enet_packet_create(&pkt, sizeof(pkt), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
        enet_peer_send(peer, 0, packet);
    }

private:
    void handlePacket(PacketHeader* header) {
        if (header->type == PacketType::JOIN_ACCEPT) {
            PacketJoinAccept* pkt = (PacketJoinAccept*)header;
            myId = pkt->assignedId;
            myStartPosition = pkt->startPosition;
            hasSpawned = true;
            std::cout << "Joined successfully! My ID: " << myId << " Start Pos: " << myStartPosition.x << "," << myStartPosition.y << "\n";
        }
        else if (header->type == PacketType::PLAYER_UPDATE) {
            PacketPlayerUpdate* pkt = (PacketPlayerUpdate*)header;
            if (pkt->header.playerId != myId) {
                // Update or add other player
                otherPlayers[pkt->header.playerId] = { pkt->position, pkt->name };
            }
        }
    }

    void sendPacket(void* data, size_t size) {
        ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }
};

#endif // CLIENT_H
