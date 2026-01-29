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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt8Number profileID[16];
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(hProfile, profileID);
    cmsColorSpaceSignature space = cmsGetColorSpace(hProfile);
    int csCode = _cmsLCMScolorSpace(space);

    // step 2: Configure
    cmsUInt32Number nGridPoints = (cmsUInt32Number)2;
    cmsUInt32Number inputChan = (cmsUInt32Number)3;
    cmsUInt32Number outputChan = (cmsUInt32Number)3;
    const cmsUInt16Number clut[24] = {
        0u, 32768u, 65535u,
        8192u, 40960u, 57344u,
        16384u, 49152u, 32768u,
        20480u, 61440u, 10240u,
        4096u, 12288u, 20480u,
        28672u, 36864u, 45056u,
        53248u, 61440u, 65535u,
        12345u, 23456u, 34567u
    };
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, clut);

    // step 3: Operate
    cmsCIEXYZ blackPoint;
    blackPoint.X = (cmsFloat64Number)0.0;
    blackPoint.Y = (cmsFloat64Number)0.0;
    blackPoint.Z = (cmsFloat64Number)0.0;
    cmsBool detected = cmsDetectDestinationBlackPoint(&blackPoint, hProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    profileID[0] = (cmsUInt8Number)(profileID[0] ^ ((cmsUInt8Number)((int)blackPoint.X & 0xFF)));
    profileID[1] = (cmsUInt8Number)(profileID[1] ^ (cmsUInt8Number)(csCode & 0xFF));
    profileID[2] = (cmsUInt8Number)(detected ? 1u : 0u);

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    /* API sequence test completed successfully */
    return 66;
}