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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE inProfile = cmsCreateNULLProfile();
    cmsHPROFILE outProfile = cmsCreateXYZProfile();
    cmsPipeline * lut = (cmsPipeline *)NULL;
    cmsUInt32Number inputFormat = (cmsUInt32Number)1;
    cmsUInt32Number outputFormat = (cmsUInt32Number)1;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;
    cmsHTRANSFORM transform = cmsCreateTransform(inProfile, inputFormat, outProfile, outputFormat, intent, flags);

    // step 2: Configure
    unsigned char membuf[1024];
    memset(membuf, 0, sizeof(membuf));
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)sizeof(membuf);
    cmsBool okMem = cmsSaveProfileToMem(outProfile, (void *)membuf, &bytesNeeded);
    (void)okMem;

    // step 3: Operate
    const char * filename = "temp_profile.icc";
    cmsBool okFile = cmsSaveProfileToFile(outProfile, filename);
    (void)okFile;
    cmsPipelineFree(lut);

    // step 4: Validate & Cleanup
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    /* API sequence test completed successfully */
    return 66;
}