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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize - create a simple profile (sRGB)
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();

    // step 2: Configure - get the D50 reference white
    const cmsCIEXYZ * d50 = cmsD50_XYZ();

    // step 3: Operate and Validate - inspect profile tags and read the first tag
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    cmsTagSignature firstSig = cmsGetTagSignature(hProfile, 0);
    void * firstTagData = cmsReadTag(hProfile, firstSig);
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number fmt = cmsFormatterForPCSOfProfile(hProfile, 128, 0); // request a formatter info (non-float)

    // step 4: Cleanup - save profile to a file (no branching or loops)
    cmsBool saved = cmsSaveProfileToFile(hProfile, "temp_profile.icc");

    // silence unused variable warnings
    (void) d50;
    (void) tagCount;
    (void) firstTagData;
    (void) creator;
    (void) fmt;
    (void) saved;
    (void) hProfile;
    return 66;
}