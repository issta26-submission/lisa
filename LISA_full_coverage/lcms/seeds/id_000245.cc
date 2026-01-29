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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x4D4F444CUL);
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    cmsUInt32Number combined = creator ^ flags ^ (cmsUInt32Number)0xA5A5A5A5UL;
    cmsSetHeaderFlags(hProfile, combined);
    cmsUInt32Number bufSize = 4096;
    unsigned char buffer[4096];
    cmsBool saved = cmsSaveProfileToMem(hProfile, (void *)buffer, &bufSize);
    cmsHPROFILE hProfile2 = cmsOpenProfileFromMem((const void *)buffer, bufSize);
    cmsSetHeaderModel(hProfile2, combined ^ (cmsUInt32Number)0xFFFFFFFFUL);
    (void)cmsGetEncodedCMMversion();
    (void)cmsGetProfileVersion(hProfile2);
    (void)saved;
    (void)creator;
    (void)flags;
    (void)combined;
    (void)hProfile;
    (void)hProfile2;
    return 66;
}