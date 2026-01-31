add_executable(chat_server
    src/main.cpp
)
target_include_directories(chat_server PRIVATE ../shared)
