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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE prof_srgb = cmsCreateNULLProfileTHR(ctx);
    cmsHPROFILE prof_xyz  = cmsCreateNULLProfile();
    cmsHPROFILE profiles[2];
    profiles[0] = prof_srgb;
    profiles[1] = prof_xyz;

    // step 2: Setup / Configure
    cmsColorSpaceSignature pcs = cmsGetPCS(prof_srgb);
    cmsFloat64Number prevAdapt = cmsSetAdaptationStateTHR(ctx, 1.0);
    cmsUInt32Number inputFormat  = (cmsUInt32Number)pcs | (cmsUInt32Number)0x201u;
    cmsUInt32Number outputFormat = (cmsUInt32Number)0x202u;
    cmsUInt32Number intent = (cmsUInt32Number)0;
    cmsUInt32Number flags  = (cmsUInt32Number)0;

    // step 3: Operate & Validate
    // Inspect profile version to simulate a validation step
    cmsFloat64Number ver = cmsGetProfileVersion(prof_srgb);
    (void)ver;
    (void)inputFormat;
    (void)outputFormat;
    (void)intent;
    (void)flags;

    // step 4: Cleanup / produce a "device link" profile and save it
    cmsSetAdaptationStateTHR(ctx, prevAdapt);
    cmsHPROFILE devlink = cmsCreateNULLProfileTHR(ctx);
    cmsIOHANDLER * io = cmsOpenIOhandlerFromFile(ctx, "devlink.icc", "wb");
    cmsUInt32Number written = cmsSaveProfileToIOhandler(devlink, io);
    (void)written;

    // API sequence test completed successfully
    return 66;
}