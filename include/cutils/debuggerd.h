/* system/debuggerd/debuggerd.h
**
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#ifndef __CUTILS_DEBUGGER_H
#define __CUTILS_DEBUGGER_H

#include <cutils/logd.h>
#include <sys/ptrace.h>
#include <unwind.h>
#include "utility.h"
#include "symbol_table.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUGGER_SOCKET_NAME "android:debuggerd"

typedef enum {
    // dump a crash
    DEBUGGER_ACTION_CRASH,
    // dump a tombstone file
    DEBUGGER_ACTION_DUMP_TOMBSTONE,
    // dump a backtrace only back to the socket
    DEBUGGER_ACTION_DUMP_BACKTRACE,
} debugger_action_t;

/* message sent over the socket */
typedef struct {
    debugger_action_t action;
    pid_t tid;
} debugger_msg_t;

/* Main entry point to get the backtrace from the crashing process */
extern int unwind_backtrace_with_ptrace(int tfd, pid_t pid, mapinfo *map,
                                        unsigned int sp_list[],
                                        int *frame0_pc_sane,
                                        bool at_fault);

extern void dump_registers(int tfd, int pid, bool at_fault);

extern int unwind_backtrace_with_ptrace_x86(int tfd, pid_t pid, mapinfo *map, bool at_fault);

void dump_pc_and_lr(int tfd, int pid, mapinfo *map, int unwound_level, bool at_fault);

void dump_stack_and_code(int tfd, int pid, mapinfo *map,
                         int unwind_depth, unsigned int sp_list[],
                         bool at_fault);

/* Dumps a process backtrace only to the specified file (requires root).
 * Returns 0 on success, -1 on error.
 */
int dump_backtrace_to_file(pid_t tid, int fd);

#ifdef __cplusplus
}
#endif

#endif /* __CUTILS_DEBUGGER_H */
