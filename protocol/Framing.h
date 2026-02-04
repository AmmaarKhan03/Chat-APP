#pragma once
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Read exactly n bytes from a socket (blocking). Throws on disconnect/error.
inline void read_exact(int fd, void* buf, size_t n) {
    auto* p = static_cast<std::uint8_t*>(buf);
    size_t total = 0;
    while (total < n) {
        ssize_t r = ::recv(fd, p + total, n - total, 0);
        if (r == 0) throw std::runtime_error("peer disconnected");
        if (r < 0)  throw std::runtime_error("recv failed");
        total += static_cast<size_t>(r);
    }
}

// Write exactly n bytes to a socket (blocking). Throws on disconnect/error.
inline void write_exact(int fd, const void* buf, size_t n) {
    const auto* p = static_cast<const std::uint8_t*>(buf);
    size_t total = 0;
    while (total < n) {
        ssize_t w = ::send(fd, p + total, n - total, 0);
        if (w == 0) throw std::runtime_error("peer disconnected");
        if (w < 0)  throw std::runtime_error("send failed");
        total += static_cast<size_t>(w);
    }
}

// Send a framed message: [4-byte big-endian length][payload bytes]
inline void send_frame(int fd, const std::string& payload) {
    if (payload.size() > 10 * 1024 * 1024) { // 10MB safety
        throw std::runtime_error("payload too large");
    }
    uint32_t len = static_cast<uint32_t>(payload.size());
    uint32_t be_len = htonl(len);
    write_exact(fd, &be_len, sizeof(be_len));
    if (!payload.empty()) write_exact(fd, payload.data(), payload.size());
}

// Receive a framed message and return its payload as string.
inline std::string recv_frame(int fd) {
    uint32_t be_len = 0;
    read_exact(fd, &be_len, sizeof(be_len));
    uint32_t len = ntohl(be_len);

    if (len > 10 * 1024 * 1024) { // 10MB safety
        throw std::runtime_error("incoming payload too large");
    }

    std::string payload(len, '\0');
    if (len > 0) read_exact(fd, payload.data(), len);
    return payload;
}
