/**************************************************************************
 *
 * Copyright 2011 Jose Fonseca
 * Copyright (C) 2013 Intel Corporation. All rights reversed.
 * Author: Shuang He <shuang.he@intel.com>
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

#ifndef _RETRACE_STATE_HPP__
#define _RETRACE_STATE_HPP__

#include "retrace.hpp"
#include "trace_parser.hpp"
#include "trace_callset.hpp"

namespace retrace {

class RetraceState {
public:
    RetraceState() : snapshotPrefix(""),
                     snapshotFormat(PNM_FMT),
                     snapshotInterval(0),
                     verbosity(0),
                     debug(1),
                     dumpingState(false),
                     driver(DRIVER_DEFAULT),
                     driverModule(NULL),

                     doubleBuffer(true),
                     samples( 1),

                     profiling(false),
                     profilingGpuTimes(false),
                     profilingCpuTimes(false),
                     profilingPixelsDrawn(false),
                     profilingMemoryUsage(false),
                     useCallNos(true),
                     singleThread(false),

                     frameNo(0),
                     callNo(0),
                     snapshot_no(0)
    {}

    enum SnapshotFormats {
        PNM_FMT,
        RAW_RGB,
        RAW_MD5
    }; 

    const char *snapshotPrefix;
    SnapshotFormats snapshotFormat;
    trace::CallSet snapshotFrequency;
    unsigned snapshotInterval;
    Retracer retracer;
    trace::Parser parser;
    trace::Profiler profiler;
    int verbosity;
    unsigned debug;
    bool dumpingState;
    Driver driver;
    const char *driverModule;
    bool doubleBuffer;
    unsigned samples;
    bool profiling;
    bool profilingGpuTimes;
    bool profilingCpuTimes;
    bool profilingPixelsDrawn;
    bool profilingMemoryUsage;
    bool useCallNos;
    bool singleThread;
    unsigned frameNo;
    unsigned callNo;
    unsigned snapshot_no;
    Dumper *dumper;

    void frameComplete(trace::Call &call);
    void takeSnapshot(unsigned call_no);
    
private:
    class NoDump: public Dumper
    {
    public:
        image::Image *
        getSnapshot(void) {
            return NULL;
        }

        bool
        canDump(void) {
            return false;
        }

        void
        dumpState(StateWriter &writer) {
            assert(0);
        }
    };

    NoDump defaultDumper;
};

}

#endif  // _RETRACE_STATE_HPP__
