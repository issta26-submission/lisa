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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE profile_sRGB = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE profile_Lab4 = cmsCreateLab4Profile(d50);
    cmsMLU * mlu_from_tag = NULL;
    char utf8_buf[128];
    memset(utf8_buf, 0, sizeof(utf8_buf));
    cmsUInt32Number utf8_len = 0;
    cmsFloat64Number version = 0.0;
    cmsUInt32Number validation = 0;

    // step 2: Configure profiles
    cmsColorSpaceSignature pcsXYZ = (cmsColorSpaceSignature)0x58595A20; // 'XYZ ' as PCS
    cmsSetPCS(profile_sRGB, pcsXYZ);
    cmsSetPCS(profile_Lab4, pcsXYZ);

    // step 3: Operate (read a tag, extract UTF8, combine with profile version)
    mlu_from_tag = (cmsMLU *) cmsReadTag(profile_sRGB, (cmsTagSignature)0);
    utf8_len = cmsMLUgetUTF8(mlu_from_tag, "en", "US", utf8_buf, (cmsUInt32Number) sizeof(utf8_buf));
    version = cmsGetProfileVersion(profile_sRGB);
    validation = (cmsUInt32Number)(utf8_len ^ (cmsUInt32Number)((cmsUInt32Number)(version * 100.0)));
    (void)validation;
    (void)utf8_buf;

    // step 4: Cleanup & finalize
    cmsSaveProfileToFile(profile_sRGB, "out_profile_srgb.icc");
    cmsSaveProfileToFile(profile_Lab4, "out_profile_lab4.icc");
    cmsUnregisterPlugins();

    // API sequence test completed successfully
    return 66;
}