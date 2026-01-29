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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a library context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) return 1;

    // Allocate an IT8 handle associated with the context
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 2;
    }

    // Set a double property on the IT8 handle (example property name and value)
    cmsFloat64Number exampleVal = 123.456;
    if (!cmsIT8SetPropertyDbl(hIT8, "ExampleProperty", exampleVal)) {
        // Setting a property failed, but continue to exercise other APIs
    }

    // Enumerate available data formats for this IT8 handle.
    // The API fills a char** array; we provide a pointer to receive it.
    char ** sampleNames = nullptr;
    int nFormats = cmsIT8EnumDataFormat(hIT8, &sampleNames);
    // We do not dereference sampleNames to avoid depending on allocation details.
    // Just validate returned count is non-negative.
    if (nFormats < 0) {
        // Enumeration failed; cleanup and exit with error
        cmsDeleteContext(ctx);
        return 3;
    }

    // Prepare a small (dummy) in-memory ICC profile buffer and try to open it.
    // Real use would provide a valid ICC profile; here we only demonstrate the call.
    unsigned char dummyProfile[128] = { 0 };
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(dummyProfile, (cmsUInt32Number)sizeof(dummyProfile));
    if (hProfile) {
        // Query the tag signature at index 0 (may or may not be meaningful for a dummy profile)
        cmsTagSignature sig = cmsGetTagSignature(hProfile, (cmsUInt32Number)0);
        (void)sig; // silence unused-variable warnings in environments where sig isn't used

        // Close the opened profile
        cmsCloseProfile(hProfile);
    } else {
        // Opening from memory failed; that's acceptable for a dummy buffer
    }

    // Note: There is no publicly listed "free IT8 handle" in the provided subset,
    // so release the context which typically frees associated resources.
    cmsDeleteContext(ctx);

    // Success
    return 66;
}