/*
 * Copyright (c) 2005-2014 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                              Version 99999999
//                          Tue Jul 23 15:40:45 2013
//
////////////////////////////////////////////////////////////////////////////////

#include "pse.igen.h"

#define PREFIX "IntervalTimer"

struct {
    Uns32 irq;
    Uns32 TimerRunning;
} state;

typedef union {
    Uns64 w64;
    Uns16 s16[4];
} periodT;

periodT period;

#define  STACKSIZE (1024 *32)

typedef struct threadInfoS {
    bhmThreadHandle handle;
    Uns64 timeout;
    double start;
    Uns8 stack[STACKSIZE];
} threadInfoT;
threadInfoT tinfo;

void interruptSet(Uns32 value);

void timerStart(Uns64 timeout);
void timerStop();

#define DEFAULT_FREQ_MHZ 166 // This produces 61 BogoMips

float freq_mhz = DEFAULT_FREQ_MHZ;

//
// Write the irq if the value changes
//
void interruptSet(Uns32 value) {
    if (state.irq != value) {
        state.irq = value;
        if (BHM_DIAG_HIGH) bhmMessage("I", PREFIX, "Setting irq=%d @ %eUS", state.irq, bhmGetCurrentTime());
        ppmWriteNet(handles.irq, state.irq);
    }
}

void timerExpired() {
    if (BHM_DIAG_HIGH) {
        double now = bhmGetCurrentTime();
        bhmMessage("I", PREFIX, "timerExpired time=%e\n", now);
    }

    //
    // irq is active
    //
    if (sp1_reg0_data.control.bits.ITO) {
        //
        // Generate timeout interrupt
        //
        sp1_reg0_data.status.bits.TO = 1;
        interruptSet(1);
    }
}

Uns32 getTimeout () {
    Uns32 count;
    //
    // Calculate remaining time
    //
    count = 0;

    return count;
}

void timerThread(void *user) {
    Uns64 timeout = tinfo.timeout;

    do {
        double start = bhmGetCurrentTime();
        if (BHM_DIAG_HIGH) bhmMessage("I", PREFIX, "Start Timer Begin at %e timeout=%lld US", start, timeout);

        tinfo.start = bhmGetCurrentTime();
        bhmWaitDelay(timeout);

        double end = bhmGetCurrentTime();
        if (BHM_DIAG_HIGH) bhmMessage("I", PREFIX, "End Timer Expired at %e", end);

        timerExpired();

    } while (sp1_reg0_data.control.bits.CONT);
}

void timerStop() {
    static int cnt=0;
    state.TimerRunning = 0;
    sp1_reg0_data.status.bits.RUN = 0;

    if (tinfo.handle) {
        cnt++;
        if (BHM_DIAG_HIGH) bhmMessage("I", PREFIX, "Delete Timer Thread cnt=%d", cnt);
        bhmDeleteThread(tinfo.handle);
        tinfo.handle = 0;
    }
}

void timerStart(Uns64 timeout) {
    state.TimerRunning = 1;
    sp1_reg0_data.status.bits.RUN = 1;

    // The timers actual period is one cycle greater than the value stored
    // in the period_n registers, the counter assumes the value zero
    // for one clock cycle, so never allow timeout to be zero
    tinfo.timeout = (Uns64)((double)(timeout) / (double)freq_mhz);
    if (tinfo.timeout == 0) {
        tinfo.timeout = 1;
    }

    if (BHM_DIAG_HIGH) {
        bhmMessage("I", PREFIX, "tinfo.timeout = %lld", tinfo.timeout);
        bhmMessage("I", PREFIX, "Create Timer Thread");
    }

    tinfo.handle = bhmCreateThread(timerThread, 0, "Timer", &(tinfo.stack[STACKSIZE]));
}

Uns64 clocksRemaining () {
    double now     = bhmGetCurrentTime();
    double start   = tinfo.start;
    double timeout = tinfo.timeout;
    double remain;
    //
    // Is this counting up or down
    //
    Uns64 value;
    remain = (start + timeout) - now;

    value = remain * freq_mhz;

    return value;
}


//////////////////////////////// Callback stubs ////////////////////////////////

//
// Transfer current countdown value to snap[index] register
//
PPM_REG_WRITE_CB(write_snap) {
    periodT p;
    p.w64 = clocksRemaining();
    sp1_reg0_data.snap_0.value = p.s16[0];
    sp1_reg0_data.snap_1.value = p.s16[1];
    sp1_reg0_data.snap_2.value = p.s16[2];
    sp1_reg0_data.snap_3.value = p.s16[3];
       
}

//
// Transfer current countdown to snap register
//
PPM_REG_READ_CB(read_snap) {
    Uns32 ud = (Uns32) user;

    Uns16 snapValue = 0;
    switch(ud){
    case 0:
        snapValue = sp1_reg0_data.snap_0.value;
        break;
    case 1:
        snapValue = sp1_reg0_data.snap_1.value;
        break;
    case 2:
        snapValue = sp1_reg0_data.snap_2.value;
        break;
    case 3:
        snapValue = sp1_reg0_data.snap_3.value;
        break;
    }

    return snapValue;
}

PPM_REG_WRITE_CB(write_control) {
    sp1_reg0_data.control.value = data;

    if (sp1_reg0_data.control.bits.START) {
        //
        // Start timer
        //
        sp1_reg0_data.control.bits.START = 0;
        if (state.TimerRunning == 0) {
            timerStart(period.w64);
        }
    }

    if (sp1_reg0_data.control.bits.STOP) {
        //
        // Stop timer
        //
        sp1_reg0_data.control.bits.STOP  = 0;
        if (state.TimerRunning == 1) {
            timerStop();
        }
    }
}

PPM_REG_READ_CB(read_control) {
    //
    // Only read bits are [1][0]
    //
    return sp1_reg0_data.control.value & 0x3;
}

PPM_REG_WRITE_CB(write_period) {
    Uns32 ud = (Uns32) user;


    switch(ud){
    case 0:
        sp1_reg0_data.period_0.value = data;
        period.s16[0] = data;
        break;
    case 1:
        sp1_reg0_data.period_1.value = data;
        period.s16[1] = data;
        break;
    case 2:
        sp1_reg0_data.period_2.value = data;
        period.s16[2] = data;
        break;
    case 3:
        sp1_reg0_data.period_3.value = data;
        period.s16[3] = data;
        break;
    }
}

PPM_REG_READ_CB(read_period) {
    bhmMessage("F", "UNSUPP", "read_period");
    return 0;
}

PPM_REG_WRITE_CB(write_status) {

    sp1_reg0_data.status.value = data;
    if (sp1_reg0_data.status.bits.TO == 0) {
        interruptSet(0);
    }
}

PPM_REG_READ_CB(read_status) {
    bhmMessage("F", "UNSUPP", "read_status");
    return *(Uns16*)user;
}

PPM_CONSTRUCTOR_CB(constructor) {
    periphConstructor();

    period.w64 = 10000;
    bhmUns64Attribute("timeoutPeriod",&(period.w64));   
    Uns64 frequency = DEFAULT_FREQ_MHZ * 1000000;
    bhmUns64Attribute("timerFrequency", &frequency);
    freq_mhz = frequency / 1000000;


    //
    // Calculate clock frequency, only updated if set
    //
    state.TimerRunning = 0;
    interruptSet(0);


}

