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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Create a profile (sRGB)
    cmsHPROFILE profile = cmsCreate_sRGBProfile();

    // step 2: Query some metadata
    cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(profile);
    cmsUInt32Number headerCreator = cmsGetHeaderCreator(profile);

    // step 3: Retrieve an ASCII info string (use index 0 as a valid info-type placeholder)
    char asciiBuf[128];
    cmsUInt32Number asciiLen = cmsGetProfileInfoASCII(profile, (cmsInfoType)0, "eng", "USA", asciiBuf, sizeof(asciiBuf));

    // step 4: Modify profile properties (set version and PCS as placeholders)
    cmsSetProfileVersion(profile, (cmsFloat64Number)4.30);
    cmsSetPCS(profile, (cmsColorSpaceSignature)0);

    // step 5: Cleanup
    cmsCloseProfile(profile);

    // silence unused variable warnings for the test harness
    (void) encodedVersion;
    (void) headerCreator;
    (void) asciiLen;
    (void) asciiBuf;

    return 66;
}