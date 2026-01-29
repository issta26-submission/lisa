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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR(ctx);
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    const char * key = "SampleKey";
    const char * subKey = "SampleSub";
    const char * value = "ExampleValue";
    const char * fetched = NULL;
    cmsTagSignature tag = (cmsTagSignature)0x64617461; /* 'data' */
    cmsUInt32Number size = 0;

    // step 2: Configure
    cmsSetProfileVersion(hProfile, (cmsFloat64Number)4.3);
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x52474220); /* 'RGB ' */
    cmsIT8SetPropertyMulti(hIT8, key, subKey, value);

    // step 3: Operate
    fetched = cmsIT8GetPropertyMulti(hIT8, key, subKey);
    size = (cmsUInt32Number)(strlen(fetched) + 1);
    cmsWriteRawTag(hProfile, tag, (const void *)fetched, size);

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);
    (void)ctx;
    (void)hProfile;
    (void)fetched;
    (void)tag;
    (void)size;

    // API sequence test completed successfully
    return 66;
}