#include "armv8def.h"

    .global EnableInt
    .type EnableInt,@function
EnableInt:
    MSR   DAIFClr,  #(DAIF_A | DAIF_F | DAIF_I)
    ret
    .size EnableInt,.-EnableInt

    .global DisableInt
    .type DisableInt,@function
DisableInt:
    MSR   DAIFSet,  #(DAIF_A | DAIF_F | DAIF_I)
    ret
    .size DisableInt,.-DisableInt

    .globl GetCoreID
    .type GetCoreID,@function
    .balign 4
GetCoreID:
    mrs x0, MPIDR_EL1
    ubfx    x0, x0, #0, #8              // coreid == affinity0
    ret
    .size GetCoreID,.-GetCoreID

