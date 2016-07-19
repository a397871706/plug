#ifndef _TEST_IPC_MESSAGE_GENERATOR_H_
#define _TEST_IPC_MESSAGE_GENERATOR_H_

#include <ipc/ipc_message_macros.h>

#define IPC_MESSAGE_START IPCTestMsgStart

IPC_MESSAGE_CONTROL0(TestStartGenerator)

IPC_MESSAGE_CONTROL0(TestStopGenerator)

#endif