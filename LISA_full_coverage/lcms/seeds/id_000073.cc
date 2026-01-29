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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt32Number memSize = 32U;
    unsigned char memBuf[32];
    memset(memBuf, 0, sizeof(memBuf));
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(memBuf, memSize);

    // step 2: Configure
    cmsUInt16Number values[2];
    values[0] = 0U;
    values[1] = 65535U;
    cmsToneCurve * curves[3];
    curves[0] = cmsBuildTabulatedToneCurve16(ctx, 2U, values);
    curves[1] = cmsBuildTabulatedToneCurve16(ctx, 2U, values);
    curves[2] = cmsBuildTabulatedToneCurve16(ctx, 2U, values);
    cmsDICTentry dictEntry;
    memset(&dictEntry, 0, sizeof(dictEntry));

    // step 3: Operate and validate
    const cmsDICTentry * nextEntry = cmsDictNextEntry(&dictEntry);
    cmsBool isMono = cmsIsToneCurveMonotonic(curves[0]);
    int csIndex = _cmsLCMScolorSpace((cmsColorSpaceSignature)isMono);

    // step 4: Cleanup
    cmsFreeToneCurveTriple(curves);
    cmsDeleteContext(ctx);
    hProfile = NULL;

    (void) nextEntry;
    (void) csIndex;
    (void) isMono;

    // API sequence test completed successfully
    return 66;
}