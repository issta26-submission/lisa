#include <cstddef>
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cassert>

// Header
int test_api_sequence() {
// step 1: Declarations & Context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    assert(ctx != NULL);
    assert(cmsGetContextUserData(ctx) == NULL);
    const char* data = "IT8.7/1\nDESCRIPTOR \"D\"\nORIGINATOR \"O\"\nKEY 123";
    cmsUInt32Number len = (cmsUInt32Number)(strlen(data) + 1);
// step 2: Load & Validate Handle
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, (void*)data, len);
    assert(hIT8 != NULL);
    assert(cmsIT8TableCount(hIT8) == 1); 
    (void)len;
// step 3: Core Property Invariants
    const char* sheetType = cmsIT8GetSheetType(hIT8);
    assert(sheetType != NULL);
    assert(strcmp(sheetType, "IT8.7/1") == 0);
    assert(strlen(sheetType) == 7);
    (void)sheetType;
    const char* desc = cmsIT8GetProperty(hIT8, "DESCRIPTOR");
    assert(desc != NULL);
    assert(strcmp(desc, "D") == 0);
    assert(desc[0] == 'D');
    const char* val = cmsIT8GetProperty(hIT8, "KEY");
    assert(val != NULL); 
    assert(strcmp(val, "123") == 0);
    assert(strlen(val) == 3);
    assert(val[0] == '1' && val[1] == '2' && val[2] == '3');
    cmsFloat64Number dblVal = cmsIT8GetPropertyDbl(hIT8, "KEY");
    assert(dblVal == 123.0);
    assert(dblVal > 0.0);
    (void)val;
    (void)dblVal;
// step 4: State Mutation & Verification
    cmsBool setRes = cmsIT8SetPropertyStr(hIT8, "NEW_PROP", "TEST");
    assert(setRes == 1); // TRUE
    const char* newVal = cmsIT8GetProperty(hIT8, "NEW_PROP");
    assert(newVal != NULL);
    assert(strcmp(newVal, "TEST") == 0);
    assert(cmsIT8GetPropertyDbl(hIT8, "KEY") == 123.0);
// step 5: Cleanup
    cmsIT8Free(hIT8);
    assert(ctx != NULL);
    cmsDeleteContext(ctx);

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