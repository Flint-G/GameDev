#ifndef SERVER_H
#define SERVER_H

#include "NetworkManager.h"
#include "Packet.h"
#include <map>

struct PlayerInfo {
    uint32_t id;
    glm::vec2 position;
    // ... other state
};

class Server : public NetworkManager {
public:
    Server() : nextPlayerId(1) {}

    bool start() {
        if (!init()) return false;

        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = SERVER_PORT;

        host = enet_host_create(&address, MAX_CLIENTS, 2, 0, 0);
        if (host == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet server host.\n";
            return false;
        }
        std::cout << "Server started on port " << SERVER_PORT << std::endl;
        return true;
    }

    void update() {
        ENetEvent event;
        while (enet_host_service(host, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    // Client connected, wait for JOIN_REQUEST
                    std::cout << "Client connected (awaiting join request)...\n";
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    handlePacket(event.peer, (PacketHeader*)event.packet->data);
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    if (event.peer->data) {
                        uint32_t id = *(uint32_t*)event.peer->data;
                        std::cout << "Client " << id << " disconnected.\n";
                        players.erase(id);
                        delete (uint32_t*)event.peer->data;
                        event.peer->data = NULL;
                    } else {
                         std::cout << "Client disconnected.\n";
                    }
                    break;
                
                default:
                    break;
            }
        }
        
        // Broadcast game state (positions) to all
        // For now, just relaying. In more complex server, we would validate.
        for (auto const& [id, player] : players) {
             PacketPlayerUpdate update;
             update.header.type = PacketType::PLAYER_UPDATE;
             update.header.playerId = id;
             update.position = player.position;
             broadcastPacket(&update, sizeof(update), id); // Don't send back to source usually, but here fine
        }
    }

private:
    uint32_t nextPlayerId;
    std::map<uint32_t, PlayerInfo> players;

    void handlePacket(ENetPeer* peer, PacketHeader* header) {
        if (header->type == PacketType::JOIN_REQUEST) {
            PacketJoinRequest* req = (PacketJoinRequest*)header;
            
            // Assign ID
            uint32_t id = nextPlayerId++;
            
            // Assign Slot logic (Grid Start)
            // Slot 0: Left Front
            // Slot 1: Right Front
            // Slot 2: Left Back
            // Slot 3: Right Back
            
            int slot = (id - 1) % 4;
            
            float xPos = 0.0f;
            float yPos = -0.8f; // Default Y is around -0.8

            if (slot % 2 == 0) xPos = -0.25f; // Left Lane (Inner)
            else               xPos =  0.25f; // Right Lane (Inner)

            // Stagger rows
            if (slot >= 2) {
                yPos -= 0.6f; // Behind
            }

            glm::vec2 spawnPos(xPos, yPos);

            // Store info
            peer->data = new uint32_t(id);
            players[id] = {id, spawnPos};
            
            std::cout << "Player " << req->name << " joined with ID " << id << " on slot " << slot << "\n";

            // Send Accept
            PacketJoinAccept ack;
            ack.header.type = PacketType::JOIN_ACCEPT;
            ack.header.playerId = 0;
            ack.assignedId = id;
            ack.totalPlayers = players.size();
            ack.startPosition = spawnPos;
            snprintf(ack.assignedName, 32, "%s", req->name);
            
            sendPacket(peer, &ack, sizeof(ack));
        }
        else if (header->type == PacketType::PLAYER_UPDATE) {
            PacketPlayerUpdate* pkt = (PacketPlayerUpdate*)header;
            // Update internal state
            if (players.find(pkt->header.playerId) != players.end()) {
                players[pkt->header.playerId].position = pkt->position;
            }
        }
    }

    void sendPacket(ENetPeer* peer, void* data, size_t size) {
        ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }
    
    void broadcastPacket(void* data, size_t size, uint32_t excludeId = 0) {
        (void)excludeId;
        ENetPacket* packet = enet_packet_create(data, size, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
        // enet_host_broadcast(host, 0, packet); 
        // We might want to exclude sender? For now broadcast all is easier to implement default enet_host_broadcast
        enet_host_broadcast(host, 0, packet);
    }
};

#endif // SERVER_H
