add_executable(chat_client
    src/main.cpp
)
target_include_directories(chat_client PRIVATE ../shared)
