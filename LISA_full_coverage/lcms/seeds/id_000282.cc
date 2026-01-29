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
//<ID> 282
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
    cmsUInt32Number nGridPoints = (cmsUInt32Number)3;
    cmsUInt32Number inputChan = (cmsUInt32Number)3;
    cmsUInt32Number outputChan = (cmsUInt32Number)3;
    cmsUInt16Number clutTable[27] = {
        0u, 32768u, 65535u,
        16384u, 49152u, 8191u,
        4096u, 20480u, 61440u,
        1024u, 512u, 256u,
        12345u, 22222u, 33333u,
        44444u, 55555u, 11111u,
        2222u, 4444u, 6666u,
        8888u, 9999u, 7777u,
        13579u, 24680u, 31415u
    };

    // step 2: Configure
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, clutTable);
    cmsCIEXYZ blackPoint;
    blackPoint.X = (cmsFloat64Number)0.0;
    blackPoint.Y = (cmsFloat64Number)0.0;
    blackPoint.Z = (cmsFloat64Number)0.0;
    cmsBool detected = cmsDetectDestinationBlackPoint(&blackPoint, hProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate & Query
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);
    int csid = _cmsLCMScolorSpace(cs);
    cmsUInt8Number summary = (cmsUInt8Number)((profileID[0] + (cmsUInt8Number)(detected ? 1u : 0u) + (cmsUInt8Number)(csid & 0xFF)) & 0xFFu);
    clutTable[0] = (cmsUInt16Number)((clutTable[0] & 0xFF00u) | (cmsUInt16Number)summary);

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(hProfile);
    // API sequence test completed successfully
    return 66;
}