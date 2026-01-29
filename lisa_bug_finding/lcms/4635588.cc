// Header
#include <cstddef>
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cassert>

int test_api_sequence() {
// step 1: Declarations
    cmsContext ctx = NULL;
    cmsUInt32Number intents[4] = {0, 0, 0, 0};
    cmsBool bpc[4] = {0, 0, 0, 0};
    cmsFloat64Number adaptation[4] = {0, 0, 0, 0};
    cmsUInt32Number nProfiles = 0;
    cmsHTRANSFORM xform = NULL;
    cmsHPROFILE profile = NULL;
    cmsFloat64Number version = 0.0;
    cmsUInt32Number supportedIntents[4];
    char *supportedDesc[4];
    assert(ctx == NULL);
    assert(xform == NULL);
    assert(profile == NULL);
    assert(version == 0.0);
    assert(nProfiles == 0); 
    assert(intents[0] == 0); 
    assert(intents[1] == 0);
    assert(intents[2] == 0); 
    assert(intents[3] == 0);
    assert(bpc[0] == 0);
    assert(bpc[1] == 0);
    assert(bpc[2] == 0); 
    assert(bpc[3] == 0);
    assert(adaptation[0] == 0); 
    assert(adaptation[1] == 0);
    assert(adaptation[2] == 0); 
    assert(adaptation[3] == 0);
    memset(supportedIntents, 0, sizeof(supportedIntents));
    memset(supportedDesc, 0, sizeof(supportedDesc));
    assert(supportedIntents[0] == 0);
    assert(supportedDesc[0] == NULL);
// step 2: Setup
    cmsUInt32Number nReturned = cmsGetSupportedIntentsTHR(ctx, 4, supportedIntents, supportedDesc);
    assert(nReturned > 0); 
    assert(supportedIntents[0] < 100); // Sanity check for intent codes
    cmsSetLogErrorHandler(NULL);
// step 3: Core operations
    xform = cmsCreateExtendedTransform(ctx, nProfiles, NULL, bpc, intents, adaptation, NULL, 0, 0, 0, 0);
    assert(xform == NULL);
    profile = cmsCreateNULLProfile();
    assert(profile != NULL); 
    version = cmsGetProfileVersion(profile);
    assert(version > 0.0);
    assert(version < 10.0); // Sanity check upper bound
// step 4: Cleanup
    cmsCloseProfile(profile);
    assert(nReturned > 0);
    assert(version > 0.0);

// API sequence test completed successfully
    (void)nReturned;
    (void)supportedDesc;
    (void)supportedIntents;

    return 66;
}

int main() {
    for (int i = 0; i < 10; i++) {
        int result = test_api_sequence();
        if (result != 66) {
            std::cerr << "API sequence test failed with error code: " << result << std::endl;
            exit(100);
        }
    }
    printf("Test completed successfully.\n");
    return 0;
}