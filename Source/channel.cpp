#include "channel.h"

namespace CH
{
    void send(Channel &data, Message message)
    {
        u32 &tailIndex = data.queue.emptyMessageTailIndex;
        u32 &headIndex = data.queue.emptyMessageHeadIndex;
        data.queue.queue[tailIndex] = message;
        if ((tailIndex + 1) % MAX_QUEUED_MESSAGES == headIndex)
        {
            return;
        }
        data.queue.messagePosted = true;
        tailIndex = (tailIndex + 1) % MAX_QUEUED_MESSAGES;
    }

    Message recv(Channel &data)
    {
        u32 &tailIndex = data.queue.emptyMessageTailIndex;
        u32 &headIndex = data.queue.emptyMessageHeadIndex;
        if ((headIndex + 1) % MAX_QUEUED_MESSAGES == tailIndex)
        {
            return Message{.type = MessageType::None};
        }
        headIndex = (headIndex + 1) % MAX_QUEUED_MESSAGES;
        Message ret = data.queue.queue[headIndex];
        data.queue.queue[headIndex].type = MessageType::None;
        data.queue.messagePosted = false;
        return ret;
    }
}