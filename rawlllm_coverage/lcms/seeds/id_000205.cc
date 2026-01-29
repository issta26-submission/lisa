#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a simple pipeline with 3 input and 3 output channels (use default context)
    cmsPipeline * orig = cmsPipelineAlloc((cmsContext) NULL, 3, 3);
    // Duplicate the pipeline using the required API
    cmsPipeline * dup = NULL;
    if (orig) {
        dup = cmsPipelineDup(orig);
    }

    // Build a simple gamma tone curve and evaluate it
    cmsToneCurve * tcurve = cmsBuildGamma((cmsContext) NULL, 2.2);
    cmsFloat32Number evalResult = 0.0f;
    if (tcurve) {
        evalResult = cmsEvalToneCurveFloat(tcurve, 0.5f);
        // free the tone curve when done
        cmsFreeToneCurve(tcurve);
        tcurve = NULL;
    }

    // Prepare a CIELab value and encode it to 16-bit representation
    cmsCIELab lab;
    lab.L = 50.0;
    lab.a = 10.0;
    lab.b = -20.0;
    cmsUInt16Number encodedLab[3] = {0, 0, 0};
    cmsFloat2LabEncodedV2(encodedLab, &lab);

    // Create a minimal temporary IT8 file to load (some implementations accept an empty/minimal file)
    const char * tmpName = "tmp_test.it8";
    FILE * tmp = fopen(tmpName, "wb");
    if (tmp) {
        // write a tiny header so the loader has something to read
        const char header[] = "IT8\n";
        fwrite(header, 1, sizeof(header) - 1, tmp);
        fclose(tmp);
    }

    // Load IT8 from file and query table count and a sample value
    cmsHANDLE hIT8 = cmsIT8LoadFromFile((cmsContext) NULL, tmpName);
    cmsUInt32Number tableCount = 0;
    cmsFloat64Number sampleVal = 0.0;
    if (hIT8) {
        tableCount = cmsIT8TableCount(hIT8);
        // Attempt to get a double value for a patch/sample. Use placeholder names.
        sampleVal = cmsIT8GetDataDbl(hIT8, "PATCH1", "SAMPLE1");
    }

    // Use results in a trivial way to avoid unused-variable warnings
    volatile int sink = 0;
    sink += (dup != NULL) ? 1 : 0;
    sink += (int)evalResult;
    sink += encodedLab[0] + encodedLab[1] + encodedLab[2];
    sink += (int)tableCount;
    sink += (int)sampleVal;

    // Return success code as requested
    (void)sink; // keep compiler happy in release builds
    return 66;
}