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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE labProfile = cmsCreateLab4Profile(d50);
    cmsHPROFILE bchProfile = cmsCreateBCHSWabstractProfile((cmsUInt32Number)17, (cmsFloat64Number)1.0, (cmsFloat64Number)1.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsHTRANSFORM xform = cmsCreateTransform(labProfile, (cmsUInt32Number)0, bchProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsUInt32Number bufSize = (cmsUInt32Number)65536;
    void * mem = malloc((size_t)bufSize);
    memset(mem, 0, (size_t)bufSize);
    cmsBool saved = cmsSaveProfileToMem(bchProfile, mem, &bufSize);

    // step 3: Operate
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsBool detected = cmsDetectDestinationBlackPoint(&blackPoint, bchProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    unsigned char input[16];
    unsigned char output[16];
    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(output));
    cmsDoTransform(xform, input, output, (cmsUInt32Number)1);

    // step 4: Validate & Cleanup
    free(mem);
    cmsCloseProfile(bchProfile);
    cmsCloseProfile(labProfile);
    (void)d50;
    (void)saved;
    (void)detected;
    (void)input;
    (void)output;
    // API sequence test completed successfully
    return 66;
}