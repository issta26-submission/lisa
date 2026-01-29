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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsHPROFILE profile = cmsCreateNULLProfile();
    cmsContext ctx = cmsGetProfileContextID(profile);
    cmsMLU * mlu = cmsMLUalloc(ctx, (cmsUInt32Number)1);
    cmsMLUsetUTF8(mlu, "eng", "USA", "Sample UTF8");
    char asciiBuf[64];
    cmsUInt32Number got = cmsMLUgetASCII(mlu, "eng", "USA", asciiBuf, (cmsUInt32Number)sizeof(asciiBuf));
    char outLang[3] = {0,0,0};
    char outCountry[3] = {0,0,0};
    cmsMLUgetTranslation(mlu, "eng", "USA", outLang, outCountry);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, 2.2);
    cmsFloat64Number est = cmsEstimateGamma(gamma, 0.001);
    cmsHPROFILE srgb = cmsCreate_sRGBProfile();
    cmsBool md5ok = cmsMD5computeID(profile);
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsBool queryOk = cmsSaveProfileToMem(profile, NULL, &bytesNeeded);
    cmsUInt32Number allocSize = bytesNeeded + 1;
    unsigned char * memBuffer = (unsigned char *)malloc((size_t)allocSize);
    memset(memBuffer, 0, (size_t)allocSize);
    cmsBool saved = cmsSaveProfileToMem(profile, (void *)memBuffer, &bytesNeeded);
    cmsUnregisterPluginsTHR(ctx);
    free(memBuffer);
    (void)got;
    (void)outLang;
    (void)outCountry;
    (void)est;
    (void)srgb;
    (void)md5ok;
    (void)queryOk;
    (void)saved;
    (void)mlu;
    (void)gamma;
    (void)profile;
    (void)ctx;
    return 66;
}