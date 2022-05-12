#pragma once

#include "Typedefs.h"

namespace CH
{
    // Threading message types
    struct ConstructedWindowInfo
    {
        u32 width = 0;
        u32 height = 0;
        HWND hwnd = 0;
        HINSTANCE hinstance = 0;
        u32 windowIndex = 0;
        bool vsync = false;
    };

    struct UpdatedWindowInfo
    {
        u32 width = 0;
        u32 height = 0;
        u32 windowIndex = 0;
    };

    struct DestroyedWindowInfo
    {
        u32 windowIndex = 0;
    };

    enum MessageType
    {
        None,
        CreateWindowMessage,
        UpdateWindowMessage,
        DestroyWindowMessage
    };

    union MessageContent
    {
        ConstructedWindowInfo create;
        UpdatedWindowInfo update;
        DestroyedWindowInfo destroy;
    };

    // Used for broadcasting messages to other threads
    struct Message
    {
        MessageType type = {};
        MessageContent content = {};
    };

    // Used to store messages for posting and reading
    struct MessageQueue
    {
        Message queue[MAX_QUEUED_MESSAGES]{}; // Ring buffer
        u32 emptyMessageHeadIndex = 0;        // Point at which elements are removed from
        u32 emptyMessageTailIndex = 1;        // Point where elements are pushed to
        bool messagePosted = false;
    };

    struct Channel
    {
        MessageQueue queue{};
    };

    void send(Channel &data, Message message);
    Message recv(Channel &data);
}
