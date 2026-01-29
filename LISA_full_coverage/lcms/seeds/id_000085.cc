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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE hSRGB = NULL;
    cmsHPROFILE hGray = NULL;
    cmsToneCurve * tone = NULL;
    cmsToneCurve * joined = NULL;
    cmsMLU * mlu = NULL;
    cmsUInt32Number intents[8] = {0};
    char * descriptions[8] = {0};
    wchar_t wbuf[64] = {0};
    char utf8buf[64] = {0};
    cmsUInt32Number count = 0;
    cmsBool ok = 0;
    float evalf = 0.0f;

    tone = cmsBuildParametricToneCurve(ctx, 1, (const cmsFloat64Number[]){2.2});
    joined = cmsJoinToneCurve(ctx, tone, tone, (cmsUInt32Number)256);
    mlu = cmsMLUalloc(ctx, (cmsUInt32Number)1);
    ok = cmsMLUsetWide(mlu, "eng", "USA", L"Example MLU string");
    count = cmsMLUgetUTF8(mlu, "eng", "USA", utf8buf, (cmsUInt32Number)sizeof(utf8buf));
    count = cmsMLUgetWide(mlu, "eng", "USA", wbuf, (cmsUInt32Number)(sizeof(wbuf)/sizeof(wbuf[0])));
    hSRGB = cmsCreate_sRGBProfile();
    cmsSetEncodedICCversion(hSRGB, (cmsUInt32Number)0x02100000);
    cmsSaveProfileToStream(hSRGB, stdout);
    cmsGetSupportedIntents((cmsUInt32Number)8, intents, descriptions);
    hGray = cmsCreateGrayProfile(NULL, joined);
    evalf = cmsEvalToneCurveFloat(joined, (cmsFloat32Number)0.5f);
    cmsFreeToneCurve(tone);
    cmsFreeToneCurve(joined);

    (void)ctx;
    (void)mlu;
    (void)ok;
    (void)count;
    (void)utf8buf;
    (void)wbuf;
    (void)descriptions;
    (void)intents;
    (void)hSRGB;
    (void)hGray;
    (void)evalf;

    return 66;
}