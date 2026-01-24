#ifndef PACKET_H
#define PACKET_H

#include <cstdint>
#include <glm/glm.hpp>

// Define port and typical constants
constexpr int SERVER_PORT = 1234;
constexpr int MAX_CLIENTS = 4;

enum class PacketType : uint8_t {
    JOIN_REQUEST = 0,
    JOIN_ACCEPT,
    PLAYER_UPDATE, // Position/Input update
    LOBBY_STATE,   // Server tells client about lobby (e.g. "Waiting", "Starting")
    START_RACE,
    DISCONNECT
};

#pragma pack(push, 1)

struct PacketHeader {
    PacketType type;
    uint32_t playerId; // 0 for server, or assigned ID for client
};

// Client sends this to request joining
struct PacketJoinRequest {
    PacketHeader header;
    char name[32];
};

// Server sends this back to assign ID
struct PacketJoinAccept {
    PacketHeader header;
    uint32_t assignedId;
    int totalPlayers;
    glm::vec2 startPosition;
    char assignedName[32]; // Server confirms name
};

// Used for both:
struct PacketPlayerUpdate {
    PacketHeader header;
    glm::vec2 position;
    float rotation; 
    float inputThrottle;
    float inputSteer;
    char name[32]; // Send name with update (bit heavy but simplest for now)
};

struct PacketLobbyState {
    PacketHeader header;
    bool isRacing; // false = lobby/waiting, true = racing
    int connectedPlayers;
};

#pragma pack(pop)

#endif // PACKET_H
