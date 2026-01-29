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
    // Step 1: Declarations & Initialization
    FILE * stream = tmpfile();
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number)0x0001U;
    alarmCodes[1] = (cmsUInt16Number)0x00FFU;

    // Step 2: Configure
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // Step 3: Operate
    cmsBool saved = cmsSaveProfileToFile(hProfile, "tmp_profile_icc_for_fuzz.icc");
    cmsFloat64Number tacValue = cmsDetectRGBProfileGamma(hProfile, 0.5);

    // Query a few harmless header/profile properties
    cmsUInt32Number iccVersion = cmsGetEncodedICCversion(hProfile);
    cmsUInt32Number cmmVersion = cmsGetEncodedCMMversion();
    cmsProfileClassSignature devClass = cmsGetDeviceClass(hProfile);
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);

    // Step 4: Validate & Cleanup
    fclose(stream);

    (void)saved;
    (void)tacValue;
    (void)iccVersion;
    (void)cmmVersion;
    (void)devClass;
    (void)cs;
    (void)ctx;
    (void)hProfile;
    (void)alarmCodes;
    (void)stream;

    return 66;
}