// events from file 34K/events
    {0x0, CTR(0) | CTR(1), 0, "CYCLES",
     "0-0 Cycles"},
    {0x1, CTR(0) | CTR(1), 0, "INSTRUCTIONS",
     "1-0 Instructions completed"},
    {0xb, CTR(0) | CTR(1), 0, "DCACHE_MISSES",
     "11-0 Data cache misses"},
    {0x2, CTR(0), 0, "BRANCH_INSNS",
     "2-0 Branch instructions (whether completed or mispredicted)"},
    {0x3, CTR(0), 0, "JR_31_INSNS",
     "3-0 JR $31 (return) instructions executed"},
    {0x4, CTR(0), 0, "JR_NON_31_INSNS",
     "4-0 JR $xx (not $31) instructions executed (at same cost as a mispredict)"},
    {0x5, CTR(0), 0, "ITLB_ACCESSES",
     "5-0 Instruction micro-TLB accesses"},
    {0x6, CTR(0), 0, "DTLB_ACCESSES",
     "6-0 Data micro-TLB accesses"},
    {0x7, CTR(0), 0, "JTLB_INSN_ACCESSES",
     "7-0 Joint TLB instruction accesses"},
    {0x8, CTR(0), 0, "JTLB_DATA_ACCESSES",
     "8-0 Joint TLB data (non-instruction) accesses"},
    {0x9, CTR(0), 0, "ICACHE_ACCESSES",
     "9-0 Instruction cache accesses"},
    {0xa, CTR(0), 0, "DCACHE_ACCESSES",
     "10-0 Data cache accesses"},
    {0xd, CTR(0), 0, "STORE_MISS_INSNS",
     "13-0 Cacheable stores that miss in the cache"},
    {0xe, CTR(0), 0, "INTEGER_INSNS",
     "14-0 Integer instructions completed"},
    {0xf, CTR(0), 0, "LOAD_INSNS",
     "15-0 Load instructions completed (including FP)"},
    {0x10, CTR(0), 0, "J_JAL_INSNS",
     "16-0 J/JAL instructions completed"},
    {0x11, CTR(0), 0, "NO_OPS_INSNS",
     "17-0 no-ops completed, ie instructions writing $0"},
    {0x12, CTR(0), 0, "ALL_STALLS",
     "18-0 Stall cycles, including ALU and IFU"},
    {0x13, CTR(0), 0, "SC_INSNS",
     "19-0 SC instructions completed"},
    {0x14, CTR(0), 0, "PREFETCH_INSNS",
     "20-0 PREFETCH instructions completed"},
    {0x15, CTR(0), 0, "L2_CACHE_WRITEBACKS",
     "21-0 L2 cache lines written back to memory"},
    {0x16, CTR(0), 0, "L2_CACHE_MISSES",
     "22-0 L2 cache accesses that missed in the cache"},
    {0x17, CTR(0), 0, "EXCEPTIONS_TAKEN",
     "23-0 Exceptions taken"},
    {0x18, CTR(0), 0, "CACHE_FIXUP_CYCLES",
     "24-0 Cache fixup cycles (specific to the 34K family microarchitecture)"},
    {0x19, CTR(0), 0, "IFU_STALLS",
     "25-0 IFU stall cycles"},
    {0x1a, CTR(0), 0, "DSP_INSNS",
     "26-0 DSP instructions completed"},
    {0x1c, CTR(0), 0, "POLICY_EVENTS",
     "28-0 Implementation specific policy manager events"},
    {0x1d, CTR(0), 0, "ISPRAM_EVENTS",
     "29-0 Implementation specific ISPRAM events"},
    {0x1e, CTR(0), 0, "COREEXTEND_EVENTS",
     "30-0 Implementation specific CorExtend events"},
    {0x1f, CTR(0), 0, "YIELD_EVENTS",
     "31-0 Implementation specific yield events"},
    {0x20, CTR(0), 0, "ITC_LOADS",
     "32-0 ITC Loads"},
    {0x21, CTR(0), 0, "UNCACHED_LOAD_INSNS",
     "33-0 Uncached load instructions"},
    {0x22, CTR(0), 0, "FORK_INSNS",
     "34-0 Fork instructions completed"},
    {0x23, CTR(0), 0, "CP2_ARITH_INSNS",
     "35-0 CP2 arithmetic instructions completed"},
    {0x25, CTR(0), 0, "ICACHE_MISS_STALLS",
     "37-0 Stall cycles due to an instruction cache miss"},
    {0x27, CTR(0), 0, "DCACHE_MISS_CYCLES",
     "39-0 Cycles a data cache miss is outstanding, but not necessarily stalling the pipeline"},
    {0x28, CTR(0), 0, "UNCACHED_STALLS",
     "40-0 Uncached stall cycles"},
    {0x29, CTR(0), 0, "MDU_STALLS",
     "41-0 MDU stall cycles"},
    {0x2a, CTR(0), 0, "CP2_STALLS",
     "42-0 CP2 stall cycles"},
    {0x2b, CTR(0), 0, "ISPRAM_STALLS",
     "43-0 ISPRAM stall cycles"},
    {0x2c, CTR(0), 0, "CACHE_INSN_STALLS",
     "44-0 Stall cycless due to CACHE instructions"},
    {0x2d, CTR(0), 0, "LOAD_USE_STALLS",
     "45-0 Load to use stall cycles"},
    {0x2e, CTR(0), 0, "INTERLOCK_STALLS",
     "46-0 Stall cycles due to return data from MFC0, RDHWR, and MFTR instructions"},
    {0x2f, CTR(0), 0, "RELAX_STALLS",
     "47-0 Low power stall cycles (operations) as requested by the policy manager"},
    {0x30, CTR(0), 0, "IFU_FB_FULL_REFETCHES",
     "48-0 Refetches due to cache misses while both fill buffers already allocated"},
    {0x31, CTR(0), 0, "EJTAG_INSN_TRIGGERS",
     "49-0 EJTAG instruction triggerpoints"},
    {0x32, CTR(0), 0, "FSB_LESS_25_FULL",
     "50-0 FSB < 25% full"},
    {0x33, CTR(0), 0, "FSB_OVER_50_FULL",
     "51-0 FSB > 50% full"},
    {0x34, CTR(0), 0, "LDQ_LESS_25_FULL",
     "52-0 LDQ < 25% full"},
    {0x35, CTR(0), 0, "LDQ_OVER_50_FULL",
     "53-0 LDQ > 50% full"},
    {0x36, CTR(0), 0, "WBB_LESS_25_FULL",
     "54-0 WBB < 25% full"},
    {0x37, CTR(0), 0, "WBB_OVER_50_FULL",
     "55-0 WBB > 50% full"},
    {0x3e, CTR(0), 0, "READ_RESPONSE_LATENCY",
     "62-0 Read latency from miss detection until critical dword of response is returned"},
    {0x402, CTR(1), 0, "MISPREDICTED_BRANCH_INSNS",
     "2-1 Branch mispredictions"},
    {0x403, CTR(1), 0, "JR_31_MISPREDICTIONS",
     "3-1 JR $31 mispredictions"},
    {0x404, CTR(1), 0, "JR_31_NO_PREDICTIONS",
     "4-1 JR $31 not predicted (stack mismatch)."},
    {0x405, CTR(1), 0, "ITLB_MISSES",
     "5-1 Instruction micro-TLB misses"},
    {0x406, CTR(1), 0, "DTLB_MISSES",
     "6-1 Data micro-TLB misses"},
    {0x407, CTR(1), 0, "JTLB_INSN_MISSES",
     "7-1 Joint TLB instruction misses"},
    {0x408, CTR(1), 0, "JTLB_DATA_MISSES",
     "8-1 Joint TLB data (non-instruction) misses"},
    {0x409, CTR(1), 0, "ICACHE_MISSES",
     "9-1 Instruction cache misses"},
    {0x40a, CTR(1), 0, "DCACHE_WRITEBACKS",
     "10-1 Data cache lines written back to memory"},
    {0x40d, CTR(1), 0, "LOAD_MISS_INSNS",
     "13-1 Cacheable load instructions that miss in the cache"},
    {0x40e, CTR(1), 0, "FPU_INSNS",
     "14-1 FPU instructions completed (not including loads/stores)"},
    {0x40f, CTR(1), 0, "STORE_INSNS",
     "15-1 Stores completed (including FP)"},
    {0x410, CTR(1), 0, "MIPS16_INSNS",
     "16-1 MIPS16 instructions completed"},
    {0x411, CTR(1), 0, "INT_MUL_DIV_INSNS",
     "17-1 Integer multiply/divide instructions completed"},
    {0x412, CTR(1), 0, "REPLAYED_INSNS",
     "18-1 Replayed instructions"},
    {0x413, CTR(1), 0, "SC_INSNS_FAILED",
     "19-1 SC instructions completed, but store failed (because the link bit had been cleared)"},
    {0x414, CTR(1), 0, "CACHE_HIT_PREFETCH_INSNS",
     "20-1 PREFETCH instructions completed with cache hit"},
    {0x415, CTR(1), 0, "L2_CACHE_ACCESSES",
     "21-1 Accesses to the L2 cache"},
    {0x416, CTR(1), 0, "L2_CACHE_SINGLE_BIT_ERRORS",
     "22-1 Single bit errors corrected in L2"},
    {0x417, CTR(1), 0, "SINGLE_THREADED_CYCLES",
     "23-1 Cycles while one and only one TC is eligible for scheduling"},
    {0x418, CTR(1), 0, "REFETCHED_INSNS",
     "24-1 Replayed instructions sent back to IFU to be refetched"},
    {0x419, CTR(1), 0, "ALU_STALLS",
     "25-1 ALU stall cycles"},
    {0x41a, CTR(1), 0, "ALU_DSP_SATURATION_INSNS",
     "26-1 ALU-DSP saturation instructions"},
    {0x41b, CTR(1), 0, "MDU_DSP_SATURATION_INSNS",
     "27-1 MDU-DSP saturation instructions"},
    {0x41c, CTR(1), 0, "CP2_EVENTS",
     "28-1 Implementation specific CP2 events"},
    {0x41d, CTR(1), 0, "DSPRAM_EVENTS",
     "29-1 Implementation specific DSPRAM events"},
    {0x41f, CTR(1), 0, "ITC_EVENT",
     "31-1 Implementation specific yield event"},
    {0x421, CTR(1), 0, "UNCACHED_STORE_INSNS",
     "33-1 Uncached store instructions"},
    {0x423, CTR(1), 0, "CP2_TO_FROM_INSNS",
     "35-1 CP2 to/from instructions (moves, control, loads, stores)"},
    {0x425, CTR(1), 0, "DCACHE_MISS_STALLS",
     "37-1 Stall cycles due to a data cache miss"},
    {0x427, CTR(1), 0, "L2_CACHE_MISS_CYCLES",
     "39-1 Cycles a L2 miss is outstanding, but not necessarily stalling the pipeline"},
    {0x428, CTR(1), 0, "ITC_STALLS",
     "40-1 ITC stall cycles"},
    {0x429, CTR(1), 0, "FPU_STALLS",
     "41-1 FPU stall cycles"},
    {0x42a, CTR(1), 0, "COREEXTEND_STALLS",
     "42-1 CorExtend stall cycles"},
    {0x42b, CTR(1), 0, "DSPRAM_STALLS",
     "43-1 DSPRAM stall cycles"},
    {0x42d, CTR(1), 0, "ALU_TO_AGEN_STALLS",
     "45-1 ALU to AGEN stall cycles"},
    {0x42e, CTR(1), 0, "MISPREDICTION_STALLS",
     "46-1 Branch mispredict stall cycles"},
    {0x430, CTR(1), 0, "FB_ENTRY_ALLOCATED_CYCLES",
     "48-1 Cycles while at least one IFU fill buffer is allocated"},
    {0x431, CTR(1), 0, "EJTAG_DATA_TRIGGERS",
     "49-1 EJTAG Data triggerpoints"},
    {0x432, CTR(1), 0, "FSB_25_50_FULL",
     "50-1 FSB 25-50% full"},
    {0x433, CTR(1), 0, "FSB_FULL_STALLS",
     "51-1 FSB full pipeline stall cycles"},
    {0x434, CTR(1), 0, "LDQ_25_50_FULL",
     "52-1 LDQ 25-50% full"},
    {0x435, CTR(1), 0, "LDQ_FULL_STALLS",
     "53-1 LDQ full pipeline stall cycles"},
    {0x436, CTR(1), 0, "WBB_25_50_FULL",
     "54-1 WBB 25-50% full"},
    {0x437, CTR(1), 0, "WBB_FULL_STALLS",
     "55-1 WBB full pipeline stall cycles"},
    {0x43e, CTR(1), 0, "READ_RESPONSE_COUNT",
     "62-1 Read requests on miss detection"},
