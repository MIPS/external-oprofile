/*
 * Copyright 2008, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Binary implementation of the original opcontrol script due to missing tools
 * like awk, test, etc.
 *
 * Enhanced to support mips 24K core (original supported only armv6 & armv7).
 * 24K has different counter events for perf-counter 0 & 1, so that capability
 * is added to the embedded "event_info" struct. Neither ARM or MIPS supports
 * oprofile unit_mask, so that feature is not implemented.
 */

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#include "op_config.h"

#if 0
#define verbose(fmt...) printf(fmt)
#else
#define verbose(fmt...)
#endif

struct event_info {
    int id;
    int valid_ctr_mask;
    const char *name;
    const char *explanation;
}; 


/* Oprofile events tables are embedded here for all architectures */

#define CTR0    (1 << 0)
#define CTR1    (1 << 1)
#define CTR2    (1 << 2)
#define CTR3    (1 << 3)
#define CTR4    (1 << 4)


#if defined(__arm__)

/* Experiments found that using a small interval may hang the device, and the
 * more events tracked simultaneously, the longer the interval has to be.
 */
#if !defined(WITH_ARM_V7_A)
#define MAX_EVENTS 3
int min_count[MAX_EVENTS] = {150000, 200000, 250000};
#else
#define MAX_EVENTS 5
int min_count[MAX_EVENTS] = {150000, 200000, 250000, 300000, 350000};
#endif

const char *default_event = "CPU_CYCLES";

struct event_info event_info[] = {
#if !defined(WITH_ARM_V7_A)
    /* events from file events/arm/armv6/events */
    {0x00, CTR0 | CTR1, "IFU_IFETCH_MISS",
     "number of instruction fetch misses"},
    {0x01, CTR0 | CTR1, "CYCLES_IFU_MEM_STALL",
     "cycles instruction fetch pipe is stalled"},
    {0x02, CTR0 | CTR1, "CYCLES_DATA_STALL",
     "cycles stall occurs for due to data dependency"},
    {0x03, CTR0 | CTR1, "ITLB_MISS",
     "number of Instruction MicroTLB misses"},
    {0x04, CTR0 | CTR1, "DTLB_MISS",
     "number of Data MicroTLB misses"},
    {0x05, CTR0 | CTR1, "BR_INST_EXECUTED",
     "branch instruction executed w/ or w/o program flow change"},
    {0x06, CTR0 | CTR1, "BR_INST_MISS_PRED",
     "branch mispredicted"},
    {0x07, CTR0 | CTR1, "INSN_EXECUTED",
     "instructions executed"},
    {0x09, CTR0 | CTR1, "DCACHE_ACCESS",
     "data cache access, cacheable locations"},
    {0x0a, CTR0 | CTR1, "DCACHE_ACCESS_ALL",
     "data cache access, all locations"},
    {0x0b, CTR0 | CTR1, "DCACHE_MISS",
     "data cache miss"},
    {0x0c, CTR0 | CTR1, "DCACHE_WB",
     "data cache writeback, 1 event for every half cacheline"},
    {0x0d, CTR0 | CTR1, "PC_CHANGE",
     "number of times the program counter was changed without a mode switch"},
    {0x0f, CTR0 | CTR1, "TLB_MISS",
     "Main TLB miss"},
    {0x10, CTR0 | CTR1, "EXP_EXTERNAL",
     "Explict external data access"},
    {0x11, CTR0 | CTR1, "LSU_STALL",
     "cycles stalled because Load Store request queque is full"},
    {0x12, CTR0 | CTR1, "WRITE_DRAIN",
     "Times write buffer was drained"},
    {0x20, CTR0 | CTR1, "ETMEXTOUT0",
     "nuber of cycles ETMEXTOUT[0] signal was asserted"},
    {0x21, CTR0 | CTR1, "ETMEXTOUT1",
     "nuber of cycles ETMEXTOUT[1] signal was asserted"},
    {0x22, CTR0 | CTR1, "ETMEXTOUT_BOTH",
     "nuber of cycles both ETMEXTOUT [0] and [1] were asserted * 2"},
    {0xff, CTR0 | CTR1 | CTR2, "CPU_CYCLES",
     "clock cycles counter"},

#else
    /* ARM V7 events */
    /* events from file events/arm/armv7/events */
    {0x00, CTR1 | CTR2 | CTR3 | CTR4, "PMNC_SW_INCR",
     "Software increment of PMNC registers"},
    {0x01, CTR1 | CTR2 | CTR3 | CTR4, "IFETCH_MISS",
     "Instruction fetch misses from cache or normal cacheable memory"},
    {0x02, CTR1 | CTR2 | CTR3 | CTR4, "ITLB_MISS",
     "Instruction fetch misses from TLB"},
    {0x03, CTR1 | CTR2 | CTR3 | CTR4, "DCACHE_REFILL",
     "Data R/W operation that causes a refill from cache or normal cacheable memory"},
    {0x04, CTR1 | CTR2 | CTR3 | CTR4, "DCACHE_ACCESS",
     "Data R/W from cache"},
    {0x05, CTR1 | CTR2 | CTR3 | CTR4, "DTLB_REFILL",
     "Data R/W that causes a TLB refill"},
    {0x06, CTR1 | CTR2 | CTR3 | CTR4, "DREAD",
     "Data read architecturally executed (note: architecturally executed = for"
     " instructions that are unconditional or that pass the condition code)"},
    {0x07, CTR1 | CTR2 | CTR3 | CTR4, "DWRITE",
     "Data write architecturally executed"},
    {0x08, CTR1 | CTR2 | CTR3 | CTR4, "INSTR_EXECUTED",
     "All executed instructions"},
    {0x09, CTR1 | CTR2 | CTR3 | CTR4, "EXC_TAKEN",
     "Exception taken"},
    {0x0A, CTR1 | CTR2 | CTR3 | CTR4, "EXC_EXECUTED",
     "Exception return architecturally executed"},
    {0x0B, CTR1 | CTR2 | CTR3 | CTR4, "CID_WRITE",
     "Instruction that writes to the Context ID Register architecturally executed"},
    {0x0C, CTR1 | CTR2 | CTR3 | CTR4, "PC_WRITE",
     "SW change of PC, architecturally executed (not by exceptions)"},
    {0x0D, CTR1 | CTR2 | CTR3 | CTR4, "PC_IMM_BRANCH",
     "Immediate branch instruction executed (taken or not)"},
    {0x0E, CTR1 | CTR2 | CTR3 | CTR4, "PC_PROC_RETURN",
     "Procedure return architecturally executed (not by exceptions)"},
    {0x0F, CTR1 | CTR2 | CTR3 | CTR4, "UNALIGNED_ACCESS",
     "Unaligned access architecturally executed"},
    {0x10, CTR1 | CTR2 | CTR3 | CTR4, "PC_BRANCH_MIS_PRED",
     "Branch mispredicted or not predicted. Counts pipeline flushes because of misprediction"},
    {0x12, CTR1 | CTR2 | CTR3 | CTR4, "PC_BRANCH_MIS_USED",
     "Branch or change in program flow that could have been predicted"},
    {0x40, CTR1 | CTR2 | CTR3 | CTR4, "WRITE_BUFFER_FULL",
     "Any write buffer full cycle"},
    {0x41, CTR1 | CTR2 | CTR3 | CTR4, "L2_STORE_MERGED",
     "Any store that is merged in L2 cache"},
    {0x42, CTR1 | CTR2 | CTR3 | CTR4, "L2_STORE_BUFF",
     "Any bufferable store from load/store to L2 cache"},
    {0x43, CTR1 | CTR2 | CTR3 | CTR4, "L2_ACCESS",
     "Any access to L2 cache"},
    {0x44, CTR1 | CTR2 | CTR3 | CTR4, "L2_CACH_MISS",
     "Any cacheable miss in L2 cache"},
    {0x45, CTR1 | CTR2 | CTR3 | CTR4, "AXI_READ_CYCLES",
     "Number of cycles for an active AXI read"},
    {0x46, CTR1 | CTR2 | CTR3 | CTR4, "AXI_WRITE_CYCLES",
     "Number of cycles for an active AXI write"},
    {0x47, CTR1 | CTR2 | CTR3 | CTR4, "MEMORY_REPLAY",
     "Any replay event in the memory subsystem"},
    {0x48, CTR1 | CTR2 | CTR3 | CTR4, "UNALIGNED_ACCESS_REPLAY",
     "Unaligned access that causes a replay"},
    {0x49, CTR1 | CTR2 | CTR3 | CTR4, "L1_DATA_MISS",
     "L1 data cache miss as a result of the hashing algorithm"},
    {0x4A, CTR1 | CTR2 | CTR3 | CTR4, "L1_INST_MISS",
     "L1 instruction cache miss as a result of the hashing algorithm"},
    {0x4B, CTR1 | CTR2 | CTR3 | CTR4, "L1_DATA_COLORING",
     "L1 data access in which a page coloring alias occurs"},
    {0x4C, CTR1 | CTR2 | CTR3 | CTR4, "L1_NEON_DATA",
     "NEON data access that hits L1 cache"},
    {0x4D, CTR1 | CTR2 | CTR3 | CTR4, "L1_NEON_CACH_DATA",
     "NEON cacheable data access that hits L1 cache"},
    {0x4E, CTR1 | CTR2 | CTR3 | CTR4, "L2_NEON",
     "L2 access as a result of NEON memory access"},
    {0x4F, CTR1 | CTR2 | CTR3 | CTR4, "L2_NEON_HIT",
     "Any NEON hit in L2 cache"},
    {0x50, CTR1 | CTR2 | CTR3 | CTR4, "L1_INST",
     "Any L1 instruction cache access, excluding CP15 cache accesses"},
    {0x51, CTR1 | CTR2 | CTR3 | CTR4, "PC_RETURN_MIS_PRED",
     "Return stack misprediction at return stack pop (incorrect target address)"},
    {0x52, CTR1 | CTR2 | CTR3 | CTR4, "PC_BRANCH_FAILED",
     "Branch prediction misprediction"},
    {0x53, CTR1 | CTR2 | CTR3 | CTR4, "PC_BRANCH_TAKEN",
     "Any predicted branch that is taken"},
    {0x54, CTR1 | CTR2 | CTR3 | CTR4, "PC_BRANCH_EXECUTED",
     "Any taken branch that is executed"},
    {0x55, CTR1 | CTR2 | CTR3 | CTR4, "OP_EXECUTED",
     "Number of operations executed (in instruction or mutli-cycle instruction)"},
    {0x56, CTR1 | CTR2 | CTR3 | CTR4, "CYCLES_INST_STALL",
     "Cycles where no instruction available"},
    {0x57, CTR1 | CTR2 | CTR3 | CTR4, "CYCLES_INST",
     "Number of instructions issued in a cycle"},
    {0x58, CTR1 | CTR2 | CTR3 | CTR4, "CYCLES_NEON_DATA_STALL",
     "Number of cycles the processor waits on MRC data from NEON"},
    {0x59, CTR1 | CTR2 | CTR3 | CTR4, "CYCLES_NEON_INST_STALL",
     "Number of cycles the processor waits on NEON instruction queue or NEON load queue"},
    {0x5A, CTR1 | CTR2 | CTR3 | CTR4, "NEON_CYCLES",
     "Number of cycles NEON and integer processors are not idle"},
    {0x70, CTR1 | CTR2 | CTR3 | CTR4, "PMU0_EVENTS",
     "Number of events from external input source PMUEXTIN[0]"},
    {0x71, CTR1 | CTR2 | CTR3 | CTR4, "PMU1_EVENTS",
     "Number of events from external input source PMUEXTIN[1]"},
    {0x72, CTR1 | CTR2 | CTR3 | CTR4, "PMU_EVENTS",
     "Number of events from both external input sources PMUEXTIN[0] and PMUEXTIN[1]"},
    {0xFF, CTR0, "CPU_CYCLES",
     "Number of CPU cycles"},

#endif // defined(WITH_ARM_V7_A)
};

#endif // if defined(__arm__)




#if defined (__mips__)

#define MAX_EVENTS 2
int min_count[MAX_EVENTS] = {15000, 20000};
const char *default_event = "CYCLES";

struct event_info event_info[] = {
    /* events from file events/mips/24K/events */
    {0x0, CTR0 | CTR1, "CYCLES",
     "Cycles"},
    {0x1, CTR0 | CTR1, "INSTRUCTIONS",
     "Instructions completed"},
    {0x11, CTR0 | CTR1, "DCACHE_MISS",
     "Data cache misses"},
    {0x22, CTR0 | CTR1, "L2_MISSES",
     "L2 cache misses"},
    {0x2, CTR0, "BRANCHES_LAUNCHED",
     "Branch instructions launched (whether completed or mispredicted)"},
    {0x3, CTR0, "JR_31_LAUNCHED",
     "jr r31 (return) instructions launched (whether completed or mispredicted)"},
    {0x4, CTR0, "JR_NON_31_LAUNCHED",
     "jr (not r31) issues, which cost the same as a mispredict."},
    {0x5, CTR0, "ITLB_ACCESSES",
     "Instruction micro-TLB accesses"},
    {0x6, CTR0, "DTLB_ACCESSES",
     "Data micro-TLB accesses"},
    {0x7, CTR0, "JTLB_DATA_ACCESSES",
     "Joint TLB instruction accesses"},
    {0x8, CTR0, "JTLB_INSTRUCTION_ACCESSES",
     "Joint TLB data (non-instruction) accesses"},
    {0x9, CTR0, "INSTRUCTION_CACHE_ACCESSES",
     "Instruction cache accesses"},
    {0xa, CTR0, "DCACHE_ACCESSES",
     "Data cache accesses"},
    {0xe, CTR0, "INTEGER_INSNS_COMPLETED",
     "Integer instructions completed"},
    {0xf, CTR0, "LOADS_COMPLETED",
     "Loads completed (including FP)"},
    {0x10, CTR0, "J_JAL_INSNS_COMPLETED",
     "j/jal instructions completed"},
    {0x11, CTR0, "NOPS_COMPLETED",
     "no-ops completed, ie instructions writing $0"},
    {0x12, CTR0, "STALLS",
     "Stalls"},
    {0x13, CTR0, "SC_COMPLETED",
     "sc instructions completed"},
    {0x14, CTR0, "PREFETCH_COMPLETED",
     "Prefetch instructions completed"},
    {0x15, CTR0, "SCACHE_WRITEBACKS",
     "L2 cache writebacks"},
    {0x17, CTR0, "EXCEPTIONS_TAKEN",
     "Exceptions taken"},
    {0x18, CTR0, "CACHE_FIXUPS",
     "``cache fixup'' events (specific to the 24K family microarchitecture)."},
    {0x2, CTR1, "BRANCH_MISSPREDICTS",
     "Branch mispredictions"},
    {0x3, CTR1, "JR_31_MISSPREDICTS",
     "jr r31 (return) mispredictions"},
    {0x5, CTR1, "ITLB_MISSES",
     "Instruction micro-TLB misses"},
    {0x6, CTR1, "DTLB_MISSES",
     "Data micro-TLB misses"},
    {0x7, CTR1, "JTLB_INSN_MISSES",
     "Joint TLB instruction misses"},
    {0x8, CTR1, "JTLB_DATA_MISSES",
     "Joint TLB data (non-instruction) misses"},
    {0x9, CTR1, "ICACHE_MISSES",
     "Instruction cache misses"},
    {0xa, CTR1, "DCACHE_WRITEBACKS",
     "Data cache writebacks"},
    {0xe, CTR1, "FPU_INSNS_NON_LOAD_STORE_COMPLETED",
     "FPU instructions completed (not including loads/stores)"},
    {0xf, CTR1, "STORES_COMPLETED",
     "Stores completed (including FP)"},
    {0x10, CTR1, "MIPS16_INSTRUCTIONS_COMPLETED",
     "MIPS16 instructions completed"},
    {0x11, CTR1, "INTEGER_MUL_DIV_COMPLETED",
     "integer multiply/divide unit instructions completed"},
    {0x12, CTR1, "REPLAY_TRAPS_NOT_UTLB",
     "``replay traps'' (other than micro-TLB related)"},
    {0x13, CTR1, "SC_COMPLETE_BUT_FAILED",
     "sc instructions completed, but store failed (because the link bit had been cleared)."},
    {0x14, CTR1, "SUPERFLUOUS_INSTRUCTIONS",
     "``superfluous'' prefetch instructions (data was already in cache)."},
    {0x15, CTR1, "SCACHE_ACCESSES",
     "L2 cache accesses"},
};
#endif // if defined (__mips__)



int list_events; 
int show_usage;
int setup;
int quick;
int num_events;
int start;
int stop;
int reset;
int timer_mode;

int selected_events[MAX_EVENTS];
int selected_counts[MAX_EVENTS];

char kernel_range[512];
char vmlinux[512];

struct option long_options[] = {
    {"help", 0, &show_usage, 1},
    {"list-events", 0, &list_events, 1},
    {"reset", 0, &reset, 1},
    {"setup", 0, &setup, 1},
    {"quick", 0, &quick, 1},
    {"event", 1, 0, 'e'},
    {"vmlinux", 1, 0, 'v'},
    {"kernel-range", 1, 0, 'r'},
    {"start", 0, &start, 1},
    {"stop", 0, &stop, 1},
    {"shutdown", 0, 0, 'h'},
    {"status", 0, 0, 't'},
    {0, 0, 0, 0},
};


void usage()
{
    printf("\nopcontrol: usage:\n"
           "   --list-events    list event types\n"
           "   --help           this message\n"
           "   --setup          setup directories\n"
           "   --quick          setup and select CPU_CYCLES:150000\n"
           "   --status         show configuration\n"
           "   --start          start data collection\n"
           "   --stop           stop data collection\n"
           "   --reset          clears out data from current session\n"
           "   --shutdown       kill the oprofile daeman\n"
           "   --event=eventspec\n"
           "      Choose an event. May be specified multiple times.\n"
           "      eventspec is in the form of name[:count], where :\n"
           "        name:  event name, see \"opcontrol --list-events\"\n"
           "        count: reset counter value\n" 
           "   --vmlinux=file   vmlinux kernel image\n"
           "   --kernel-range=start,end\n"
           "                    kernel range vma address in hexadecimal\n"
          );
}

void setup_session_dir()
{
    int fd;

    fd = open(OP_DATA_DIR, O_RDONLY);
    if (fd != -1) {
        close(fd);
        system("rm -r "OP_DATA_DIR);
    }

    if (mkdir(OP_DATA_DIR, 755)) {
        fprintf(stderr, "Cannot create directory \"%s\": %s\n",
                OP_DATA_DIR, strerror(errno));
    }
    if (mkdir(OP_DATA_DIR"/samples", 644)) {
        fprintf(stderr, "Cannot create directory \"%s\": %s\n",
                OP_DATA_DIR"/samples", strerror(errno));
    }
}


void umount_op_device()
{
    if (system("umount "OP_DRIVER_BASE)) {
        fprintf(stderr, "Cannot umount device fs \"%s\": %s \n", 
                OP_DRIVER_BASE, strerror(errno));
    }
    /* rm mount point, so next do_setup() won't fail */
    if (rmdir(OP_DRIVER_BASE)) {
        fprintf(stderr, "Cannot remove mount point: "OP_DRIVER_BASE": %s\n",
                strerror(errno));
    }
}

int do_setup()
{
    char dir[1024];

    setup_session_dir();

    /* create the dev mountpoint if it does not already exist */
    int fd = open(OP_DRIVER_BASE, O_RDONLY);
    if (fd == -1) {
        if (mkdir(OP_DRIVER_BASE, 644)) {
            fprintf(stderr, "Cannot create directory "OP_DRIVER_BASE": %s\n",
                    strerror(errno));
            return -1;
        }
    
    } else {
        close(fd);
    }
    
    if (system("mount -t oprofilefs nodev "OP_DRIVER_BASE)) {
        return -1;
    }
    return 0;
}


void stringify_counters(char *ctr_string, int ctr_mask)
{
    int i, n, len;
    char *p = ctr_string;
    
    *p = '\0';
    for (i=0; i<32; ++i) {
        if (ctr_mask & (1<<i)) {
            p += sprintf(p, "%d,", i);
        }
    }
    if (p != ctr_string) {
        *(p-1) = '\0';  /* erase the final comma */
    }
}

void do_list_events()
{
    unsigned int i;
    char ctrs[128];

    printf("%-12s | %-30s: %s\n", "counter", "name", "meaning");
    printf("----------------------------------------"
           "--------------------------------------\n");
    for (i = 0; i < sizeof(event_info)/sizeof(struct event_info); i++) {
        stringify_counters(ctrs, event_info[i].valid_ctr_mask);
        printf("%-12s | %-30s: %s\n", ctrs, event_info[i].name, event_info[i].explanation);
    }
}

int find_event_idx_from_name(const char *name)
{
    unsigned int i;

    for (i = 0; i < sizeof(event_info)/sizeof(struct event_info); i++) {
        if (!strcmp(name, event_info[i].name)) {
            return i;
        }
    }
    return -1;
}

const char * find_event_name_from_id(int id, int ctr_mask)
{
    unsigned int i;

    for (i = 0; i < sizeof(event_info)/sizeof(struct event_info); i++) {
        if (event_info[i].id == id && event_info[i].valid_ctr_mask & ctr_mask) {
            return event_info[i].name;
        }
    }
    return "Undefined Event";
}

int process_event(const char *event_spec)
{
    char event_name[512];
    char count_name[512];
    unsigned int i;
    int event_idx;
    int count_val;

    strncpy(event_name, event_spec, 512);
    count_name[0] = 0;
    
    if (strcmp(event_name, "timer") == 0) {
        /* using timer mode for cpus without performance counters */
        timer_mode = 1;
        verbose("Using oprofile timer mode\n");
        return 0;
    }

    /* First, check if the name is followed by ":" */
    for (i = 0; i < strlen(event_name); i++) {
        if (event_name[i] == 0) {
            break;
        }
        if (event_name[i] == ':') {
            strncpy(count_name, event_name+i+1, 512);
            event_name[i] = 0;
            break;
        }
    }
    event_idx = find_event_idx_from_name(event_name);
    if (event_idx == -1) {
        fprintf(stderr, "Unknown event name: %s\n", event_name);
        return -1;
    }
    
    /* validate that the named event is valid for this event counter */
    /* some arch's don't support all named events on all counters */
    /* 'num_events' represents the cpu internal counter number */
    verbose("idx: %d, name: %s, valid_ctr: %02x, ctr#: %d\n", 
            event_idx, event_info[event_idx].name, 
            event_info[event_idx].valid_ctr_mask, num_events);
    if ( ! (1<<num_events & event_info[event_idx].valid_ctr_mask) ) {
        fprintf(stderr, "Bad event name: %s for counter %d, see --list\n", 
                event_name, num_events);
        return -1;
    }

    /* Use defualt count */
    if (count_name[0] == 0) {
        count_val = min_count[0];
    } else {
        count_val = atoi(count_name);
    }

    selected_events[num_events] = event_idx;
    selected_counts[num_events++] = count_val;
    verbose("event_id is %d\n", event_info[event_idx].id);
    verbose("count_val is %d\n", count_val);
    return 0;
}

int echo_dev(const char* str, int val, const char* file, int counter)
{
    char fullname[512];
    char content[128];
    int fd;
    
    if (counter >= 0) {
        snprintf(fullname, 512, OP_DRIVER_BASE"/%d/%s", counter, file);
    }
    else {
        snprintf(fullname, 512, OP_DRIVER_BASE"/%s", file);
    }
    fd = open(fullname, O_WRONLY);
    if (fd<0) {
        fprintf(stderr, "Cannot open %s: %s\n", fullname, strerror(errno));
        return fd;
    }
    if (str == 0) {
        sprintf(content, "%d", val);
    }
    else {
        strncpy(content, str, 128);
    }
    verbose("Configure %s (%s)\n", fullname, content);
    write(fd, content, strlen(content));
    close(fd);
    return 0;
}

int read_num(const char* file)
{
    char buffer[256];
    int fd = open(file, O_RDONLY);
    if (fd<0) return -1;
    int rd = read(fd, buffer, sizeof(buffer)-1);
    buffer[rd] = 0;
    return atoi(buffer);
}

void do_status()
{
    int num;
    char fullname[512];
    int i;

    printf("Driver directory: %s\n", OP_DRIVER_BASE);
    printf("Session directory: %s\n", OP_DATA_DIR);
    for (i = 0; i < MAX_EVENTS; i++) {
        sprintf(fullname, OP_DRIVER_BASE"/%d/enabled", i);
        num = read_num(fullname);
        if (num > 0) {
            printf("Counter %d:\n", i);

            /* event name */
            sprintf(fullname, OP_DRIVER_BASE"/%d/event", i);
            num = read_num(fullname);
            printf("    name: %s\n", find_event_name_from_id(num, (1<<i)));

            /* profile interval */
            sprintf(fullname, OP_DRIVER_BASE"/%d/count", i);
            num = read_num(fullname);
            printf("    count: %d\n", num);
        }
        else {
            printf("Counter %d disabled\n", i);
        }
    }

    num = read_num(OP_DATA_DIR"/lock");
    if (num >= 0) {
        int fd;
        /* Still needs to check if this lock is left-over */
        sprintf(fullname, "/proc/%d", num);
        fd = open(fullname, O_RDONLY);
        if (fd == -1) {
            printf("Session directory is not clean - do \"opcontrol --setup\""
                   " before you continue\n");
            return;
        }
        else {
            close(fd);
            printf("oprofiled pid: %d\n", num);
            num = read_num(OP_DRIVER_BASE"/enable");
            printf("profiler is%s running\n", num == 0 ? " not" : "");
            num = read_num(OP_DRIVER_BASE"/stats/cpu0/sample_received");
            printf("  %9u samples received\n", num);
            num = read_num(OP_DRIVER_BASE"/stats/cpu0/sample_lost_overflow");
            printf("  %9u samples lost overflow\n", num);
#if 0
            /* FIXME - backtrace seems broken */
            num = read_num(OP_DRIVER_BASE"/stats/cpu0/backtrace_aborted");
            printf("  %9u backtrace aborted\n", num);
            num = read_num(OP_DRIVER_BASE"/backtrace_depth");
            printf("  %9u backtrace_depth\n", num);
#endif
        }
    }
    else {
        printf("oprofiled is not running\n");
    }
}

void do_reset() 
{
    int fd;
    
    int pid = read_num(OP_DATA_DIR"/lock");
    if (pid >= 0) {
        kill(pid, SIGHUP);  /* HUP makes oprofiled close its sample files */
    }
    fd = open(OP_DATA_DIR"/samples/current", O_RDONLY);
    if (fd == -1) {
        return;
    }
    close(fd);
    system("rm -r "OP_DATA_DIR"/samples/current");
}

int main(int argc, char * const argv[])
{
    int option_index;
    char command[1024];

    /* Initialize default strings */
    strcpy(vmlinux, "--no-vmlinux");
    strcpy(kernel_range, "");

    while (1) {
        int c = getopt_long(argc, argv, "", long_options, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
            case 0:
                break;
            /* --event */
            case 'e':   
                if (num_events == MAX_EVENTS) {
                    fprintf(stderr, "More than %d events specified\n",
                            MAX_EVENTS);
                    exit(1);
                }
                if (process_event(optarg)) {
                    exit(1);
                }
                break;
            /* --vmlinux */
            case 'v':
                sprintf(vmlinux, "-k %s", optarg);
                break;
            /* --kernel-range */
            case 'r':
                sprintf(kernel_range, "-r %s", optarg);
                break;
            /* --shutdown */
            case 'h': {
                int pid = read_num(OP_DATA_DIR"/lock");
                if (pid >= 0) {
                    kill(pid, SIGKILL);
                }
                sleep(1);
                umount_op_device();
                break;
            }
            /* --status */
            case 't':
                do_status();
                break;
            default:
                usage();
                exit(1);
        }
    }
    verbose("list_events = %d\n", list_events);
    verbose("setup = %d\n", setup);

    if (list_events) {
        do_list_events();
    }

    if (quick) {
        process_event(default_event);
        setup = 1;
    }

    if (reset) {
        do_reset();
    }

    if (show_usage) {
        usage();
    }

    if (setup) {
        if (do_setup()) {
            fprintf(stderr, "do_setup failed");
            exit(1);
        }
    }

    if (num_events != 0  ||  timer_mode) {
        int i;

        strcpy(command, "oprofiled --session-dir="OP_DATA_DIR);

#if defined(__arm__) && !defined(WITH_ARM_V7_A)
        /* Since counter #3 can only handle CPU_CYCLES, check and shuffle the 
         * order a bit so that the maximal number of events can be profiled
         * simultaneously
         */
        if (num_events == 3) {
            for (i = 0; i < num_events; i++) {
                int event_idx = selected_events[i];

                if (event_info[event_idx].id == 0xff) {
                    break;
                }
            }

            /* No CPU_CYCLES is found */
            if (i == 3) {
                fprintf(stderr, "You can only specify three events if one of "
                                "them is CPU_CYCLES\n");
                exit(1);
            }
            /* Swap CPU_CYCLES to counter #2 (starting from #0)*/
            else if (i != 2) {
                int temp;

                temp = selected_events[2];
                selected_events[2] = selected_events[i];
                selected_events[i] = temp;

                temp = selected_counts[2];
                selected_counts[2] = selected_counts[i];
                selected_counts[i] = temp;
            }
        }
#endif

        if (timer_mode) {
            snprintf(command+strlen(command), 1024 - strlen(command), 
                     " --events=");
        }

        /* Configure the counters and enable them */
        for (i = 0; i < num_events; i++) {
            int event_idx = selected_events[i];
            int setup_result = 0;

            if (i == 0) {
                snprintf(command+strlen(command), 1024 - strlen(command), 
                         " --events=");
            }
            else {
                snprintf(command+strlen(command), 1024 - strlen(command), 
                         ",");
            }
            /* Compose name:id:count:unit_mask:kernel:user, something like
             * --events=CYCLES_DATA_STALL:2:0:200000:0:1:1,....
             */
            snprintf(command+strlen(command), 1024 - strlen(command), 
                     "%s:%d:%d:%d:0:1:1",
                     event_info[event_idx].name,
                     event_info[event_idx].id,
                     i,
                     selected_counts[i]);

            setup_result |= echo_dev("1", 0, "user", i);
            setup_result |= echo_dev("1", 0, "kernel", i);
            setup_result |= echo_dev("0", 0, "unit_mask", i);
            setup_result |= echo_dev("1", 0, "enabled", i);
            setup_result |= echo_dev(NULL, selected_counts[i], "count", i);
            setup_result |= echo_dev(NULL, event_info[event_idx].id, 
                                     "event", i);
            if (setup_result) {
                fprintf(stderr, "Counter configuration failed for %s\n",
                        event_info[event_idx].name);
                fprintf(stderr, "Did you do \"opcontrol --setup\" first?\n");
                exit(1);
            }
        }

        /* Disable the unused counters */
        if (!timer_mode) {
            for (i = num_events; i < MAX_EVENTS; i++) {
                echo_dev("0", 0, "enabled", i);
            }
        }

        snprintf(command+strlen(command), 1024 - strlen(command), " %s",
                 vmlinux);
        if (kernel_range[0]) {
            snprintf(command+strlen(command), 1024 - strlen(command), " %s",
                     kernel_range);
        }
        verbose("command: %s\n", command);
        system(command);
    }

    if (start) {
        echo_dev("1", 0, "enable", -1);
    }

    if (stop) {
        echo_dev("0", 0, "enable", -1);
    }
}
