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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE inProfile = cmsCreateXYZProfile();
    cmsHPROFILE outProfile = cmsCreateNULLProfile();
    unsigned char headerID[16];
    memset(headerID, 0, sizeof(headerID));
    cmsGetHeaderProfileID(inProfile, (cmsUInt8Number *)headerID);
    cmsColorSpaceSignature cs = cmsGetColorSpace(inProfile);
    (void)cs;

    // step 2: Configure
    cmsHTRANSFORM transform = cmsCreateTransform(inProfile, (cmsUInt32Number)0, outProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsHPROFILE devLink = cmsTransform2DeviceLink(transform, (cmsFloat64Number)4.3, (cmsUInt32Number)0);

    // step 3: Operate
    cmsUInt32Number needed = (cmsUInt32Number)0;
    cmsSaveProfileToMem(devLink, (void *)0, &needed);
    void * mem = malloc((size_t)needed);
    memset(mem, 0, (size_t)needed);
    cmsSaveProfileToMem(devLink, mem, &needed);
    cmsSaveProfileToFile(inProfile, "in_profile.icc");
    cmsSaveProfileToFile(devLink, "devlink_profile.icc");

    // step 4: Validate & Cleanup
    cmsPipelineFree((cmsPipeline *)0);
    free(mem);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    cmsCloseProfile(devLink);
    /* API sequence test completed successfully */
    return 66;
}