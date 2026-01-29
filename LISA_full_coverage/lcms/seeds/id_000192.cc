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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    unsigned char memProfile[128];
    memset(memProfile, 0, sizeof(memProfile));
    memProfile[0] = 'L';
    memProfile[1] = 'C';
    memProfile[2] = 'M';
    memProfile[3] = 'S';

    cmsHPROFILE hNull = cmsCreateNULLProfile();
    cmsHPROFILE hPlaceholder = cmsCreateProfilePlaceholder(ctx);

    // step 2: Configure header and global state
    cmsSetHeaderManufacturer(hPlaceholder, (cmsUInt32Number)0x12345678);
    cmsSetHeaderFlags(hPlaceholder, (cmsUInt32Number)0x0);
    cmsSetHeaderRenderingIntent(hPlaceholder, (cmsUInt32Number)1);
    cmsFloat64Number prevAdapt = cmsSetAdaptationStateTHR(ctx, 0.5);

    // step 3: Operate on profile
    cmsBool wrote = cmsWriteRawTag(hPlaceholder, (cmsTagSignature)0, memProfile, (cmsUInt32Number)sizeof(memProfile));
    cmsUInt32Number creator = cmsGetHeaderCreator(hPlaceholder);
    cmsFloat64Number version = cmsGetProfileVersion(hPlaceholder);
    cmsBool saved = cmsSaveProfileToFile(hPlaceholder, "fuzz_temp_profile.icc");

    // step 4: Validate & Cleanup
    cmsCloseProfile(hPlaceholder);
    cmsCloseProfile(hNull);

    (void)ctx;
    (void)memProfile;
    (void)prevAdapt;
    (void)wrote;
    (void)creator;
    (void)version;
    (void)saved;

    // API sequence test completed successfully
    return 66;
}