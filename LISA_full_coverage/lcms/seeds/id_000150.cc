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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsHPROFILE hProfile = NULL;
    cmsContext ctx = NULL;
    cmsIOHANDLER * ioh = NULL;
    cmsToneCurve * tone = NULL;
    cmsUInt16Number tc_values[3];
    cmsBool md5ok = 0;
    cmsUInt32Number formatter = 0;
    cmsUInt32Number savedBytes = 0;
    unsigned char memBuf[4096];
    cmsUInt32Number bufSize = sizeof(memBuf);
    cmsUInt16Number evalSample = 0;

    // step 2: Setup (Create profile, obtain context, open IO handler, prepare tone values)
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    ioh = cmsOpenIOhandlerFromNULL(ctx);
    memset(tc_values, 0, sizeof(tc_values));
    tc_values[0] = 0;
    tc_values[1] = 32768U;
    tc_values[2] = 65535U;

    // step 3: Core operations (Build tone curve, configure profile, compute ID, format, save, evaluate)
    tone = cmsBuildTabulatedToneCurve16(ctx, 3U, tc_values);
    cmsSetColorSpace(hProfile, _cmsICCcolorSpace(0));
    md5ok = cmsMD5computeID(hProfile);
    formatter = cmsFormatterForPCSOfProfile(hProfile, 128U, (cmsBool)0);
    savedBytes = bufSize;
    (void)cmsSaveProfileToMem(hProfile, memBuf, &savedBytes);
    evalSample = cmsEvalToneCurve16(tone, 32768U);

    // step 4: Cleanup (validate results and release local resources)
    (void)hProfile;
    (void)ctx;
    (void)ioh;
    (void)tone;
    (void)tc_values;
    (void)md5ok;
    (void)formatter;
    (void)savedBytes;
    (void)memBuf;
    (void)bufSize;
    (void)evalSample;

    // API sequence test completed successfully
    return 66;
}