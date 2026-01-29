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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE profiles[2];
    cmsHTRANSFORM xform = NULL;
    cmsUInt32Number nProfiles = (cmsUInt32Number)2;
    cmsUInt32Number inputFmt = (cmsUInt32Number)0;
    cmsUInt32Number outputFmt = (cmsUInt32Number)0;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags = (cmsUInt32Number)0;
    cmsFloat64Number savedAdapt;

    // step 2: Setup
    profiles[0] = cmsCreate_sRGBProfile();
    profiles[1] = cmsCreateXYZProfile();
    savedAdapt = cmsSetAdaptationStateTHR(ctx, (cmsFloat64Number)1.0);

    // step 3: Core operations
    xform = cmsCreateMultiprofileTransformTHR(ctx, profiles, nProfiles, inputFmt, outputFmt, intent, flags);
    cmsUInt32Number gotInputFormat = cmsGetTransformInputFormat(xform);
    cmsColorSpaceSignature pcs = cmsGetPCS(profiles[0]);
    cmsUInt32Number validation = gotInputFormat + (cmsUInt32Number)pcs;

    // step 4: Cleanup
    cmsSetAdaptationStateTHR(ctx, savedAdapt);
    cmsCloseProfile(profiles[0]);
    cmsCloseProfile(profiles[1]);
    (void)validation;
    // API sequence test completed successfully
    return 66;
}