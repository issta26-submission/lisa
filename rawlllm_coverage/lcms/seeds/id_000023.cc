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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Query supported intents
    const cmsUInt32Number MAX_INTENTS = 16;
    cmsUInt32Number codes[MAX_INTENTS];
    char * descriptions[MAX_INTENTS] = { 0 };

    cmsUInt32Number nIntents = cmsGetSupportedIntents(MAX_INTENTS, codes, descriptions);
    if (nIntents == 0) {
        // nothing supported or call failed
        return 1;
    }

    // Create a simple profile to later close
    cmsHPROFILE profile = cmsCreateNULLProfile();
    if (profile == NULL) {
        return 2;
    }

    // Allocate a minimal MLU and attempt to get an ASCII entry from it.
    // Use a null context (0) which is commonly accepted by the LCMS API for default context.
    cmsMLU * mlu = cmsMLUalloc((cmsContext)0, 1);
    if (mlu == NULL) {
        // cleanup
        cmsCloseProfile(profile);
        return 3;
    }

    char lang[3] = "en";   // 2-letter language + null
    char country[3] = "US"; // 2-letter country + null
    char buffer[128] = {0};

    cmsUInt32Number got = cmsMLUgetASCII((const cmsMLU *)mlu, lang, country, buffer, sizeof(buffer));
    // It's acceptable if nothing is found (got == 0). We treat that as non-fatal.

    // Allocate an identity stage and inspect its type
    cmsStage * stage = cmsStageAllocIdentity((cmsContext)0, 3);
    if (stage == NULL) {
        // cleanup
        cmsCloseProfile(profile);
        return 4;
    }

    // Retrieve the stage signature/type (don't rely on its value here, just exercise the API)
    cmsStageSignature stype = cmsStageType(stage);
    (void)stype; // silence unused variable warnings

    // Close the profile (exercise the close API). Treat failure as non-fatal but report if it fails.
    cmsBool closed = cmsCloseProfile(profile);
    if (!closed) {
        // we attempted to close but it didn't succeed; still consider the test accomplished
        return 5;
    }

    // Success
    return 66;
}