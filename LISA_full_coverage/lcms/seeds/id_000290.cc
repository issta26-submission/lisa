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
//<ID> 290
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
    cmsUInt32Number memCapacity = (cmsUInt32Number)16384U;
    void * memBuffer = malloc((size_t)memCapacity);
    memset(memBuffer, 0, (size_t)memCapacity);
    cmsUInt32Number bytesNeeded = memCapacity;
    cmsBool okMem = cmsSaveProfileToMem(inProfile, memBuffer, &bytesNeeded);
    cmsBool okFile = cmsSaveProfileToFile(outProfile, "test_profile.icc");
    cmsHTRANSFORM transform = cmsCreateTransform(inProfile, (cmsUInt32Number)0, outProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsPipeline * pipeline = (cmsPipeline *)NULL;

    // step 2: Configure
    cmsColorSpaceSignature inCS = cmsGetColorSpace(inProfile);
    int inCSType = _cmsLCMScolorSpace(inCS);
    (void)inCSType;
    cmsUInt32Number reportedSize = bytesNeeded;
    (void)reportedSize;

    // step 3: Operate
    cmsPipelineFree(pipeline);

    // step 4: Validate & Cleanup
    free(memBuffer);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    /* API sequence test completed successfully */
    return 66;
}