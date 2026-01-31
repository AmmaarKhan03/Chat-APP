# C++ Chat Application

A real-time chat application in C++ with:
- Private messaging
- Group chats (rooms)
- File sharing
- Online presence

## Structure
- `server/` - TCP chat server
- `client/` - CLI client (GUI later)
- `shared/` - shared protocol/models

## Build (CMake)
```bash
cmake -S . -B build
cmake --build build