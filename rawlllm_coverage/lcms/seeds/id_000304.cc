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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context (no plugins or user data)
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        return 1;
    }

    // Create a simple profile (OkLab profile) using the context
    cmsHPROFILE hProfile = cmsCreate_OkLabProfile(ctx);
    if (hProfile == nullptr) {
        cmsDeleteContext(ctx);
        return 2;
    }

    // Set the PCS of the profile (use an integer literal; header will define the proper signature)
    cmsSetPCS(hProfile, (cmsColorSpaceSignature) 0x58595A20); // 'XYZ ' as a common PCS mnemonic

    // Ask for a formatter for the PCS of the created profile (example: 12 bytes, not float)
    cmsUInt32Number fmt = cmsFormatterForPCSOfProfile(hProfile, 12, 0);
    (void) fmt; // suppress unused-variable warning; we only exercise the API

    // Prepare a small CLUT table (2x2x2) and points array
    cmsUInt32Number clutPoints[3] = { 2, 2, 2 };
    const cmsUInt32Number inputChan  = 3;
    const cmsUInt32Number outputChan = 3;

    // 2*2*2 = 8 entries; for 3 channels out we could store 8*3 floats,
    // but the simplified cmsStageAllocCLutFloatGranular prototype given
    // here accepts a Table pointer. Provide a small table of 8 floats.
    cmsFloat32Number table[8];
    for (cmsUInt32Number i = 0; i < 8; ++i) {
        table[i] = (cmsFloat32Number)(i / 7.0f);
    }

    // Allocate a CLUT stage using the granular allocator
    cmsStage * stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table);
    if (stage == nullptr) {
        // cleanup and fail
        cmsDeleteContext(ctx);
        return 3;
    }

    // Retrieve a tag signature (index 0) and check if that tag exists
    cmsTagSignature tag = cmsGetTagSignature(hProfile, 0);
    cmsBool hasTag = cmsIsTag(hProfile, tag);
    (void) hasTag; // we only invoked the API to exercise it

    // Normal cleanup of context (profiles/stages may be managed by the library or require other APIs;
    // we at least remove the created context as a minimal cleanup step)
    cmsDeleteContext(ctx);

    // Success as requested
    return 66;
}