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

#include "retrace_state.hpp"
#include "image.hpp"

using retrace::RetraceState;

void
RetraceState::frameComplete(trace::Call &call) {
    ++frameNo;

    if (!(call.flags & trace::CALL_FLAG_END_FRAME) &&
        snapshotFrequency.contains(call)) {
        // This call doesn't have the end of frame flag, so take any snapshot
        // now.
        takeSnapshot(call.no);
    }
}


void
RetraceState::takeSnapshot(unsigned call_no) {
    assert(dumpingSnapshots);
    assert(snapshotPrefix);

    image::Image *src = dumper->getSnapshot();
    if (!src) {
        std::cerr << call_no << ": warning: failed to get snapshot\n";
        return;
    }

    if ((snapshotInterval == 0 ||
         (snapshot_no % snapshotInterval) == 0)) {

        if (snapshotPrefix[0] == '-' && snapshotPrefix[1] == 0) {
            char comment[21];
            snprintf(comment, sizeof comment, "%u",
                     useCallNos ? call_no : snapshot_no);
            switch (snapshotFormat) {
            case PNM_FMT:
                src->writePNM(std::cout, comment);
                break;
            case RAW_RGB:
                src->writeRAW(std::cout);
                break;
            case RAW_MD5:
                src->writeMD5(std::cout);
                break;
            default:
                assert(0);
                break;
            }
        } else {
            os::String filename = os::String::format("%s%010u.png",
                                                     snapshotPrefix,
                                                     useCallNos ? call_no : snapshot_no);

            // Alpha channel often has bogus data, so strip it when writing
            // PNG images to disk to simplify visualization.
            bool strip_alpha = true;

            if (src->writePNG(filename, strip_alpha) &&
                retrace::verbosity >= 0) {
                std::cout << "Wrote " << filename << "\n";
            }
        }
    }

    delete src;

    snapshot_no++;

    return;
}
