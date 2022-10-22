#pragma once

#include "typedefs.h"
#include "fixed_array.h"
#include "win.h"

namespace std
{
    template <u32 C>
    class ThreadPool
    {
    public:
        class Thread
        {
        public:
            HANDLE thread = 0;
            DWORD id = 0;

            Thread() {}
            Thread(u32 stackSize, ThreadProc startAddress, void *parameter, u32 creationFlags) { thread = CreateThread(0, stackSize, startAddress, parameter, creationFlags, &id); }
            ~Thread() {}
        };

        HANDLE ioCompletionPort{};
        FixedArray<Thread, C> workerThreads{};

        ThreadPool() { ioCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0); }
        ~ThreadPool()
        {
            for (u32 i = 0; i < C; i++)
            {
                workerThreads[i].~Thread();
            }
            CloseHandle(ioCompletionPort);
        }

        void createThread(u32 stackSize, ThreadProc startAddress, void *parameter, u32 creationFlags)
        {
            workerThreads.emplace_back(stackSize, startAddress, parameter, creationFlags);
        }

        void destroyThread(u32 index)
        {
            if (index != 0)
            {
                workerThreads.replace_with_back(index);
            }
            else
            {
                workerThreads.pop_back();
            }
        }
    };
}