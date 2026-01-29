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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations and allocations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE prof_srgb = cmsCreate_sRGBProfileTHR(ctx);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE prof_lab = cmsCreateLab4Profile(d50);
    cmsMLU * mlu = (cmsMLU *) cmsReadTag(prof_srgb, (cmsTagSignature)0x64657363); // 'desc'
    char utf8buf[128];
    memset(utf8buf, 0, sizeof(utf8buf));

    // step 2: Configure profiles and extract textual data
    cmsSetPCS(prof_srgb, (cmsColorSpaceSignature)0x58595A20); // 'XYZ '
    cmsUInt32Number copied = cmsMLUgetUTF8(mlu, "en", "US", utf8buf, (cmsUInt32Number)sizeof(utf8buf));
    (void)copied;

    // step 3: Operate / validate using profile metadata and D50 whitepoint
    cmsFloat64Number version = cmsGetProfileVersion(prof_srgb);
    cmsFloat64Number wpY = d50->Y;
    cmsFloat64Number checksum = version + wpY;
    (void)checksum;
    (void)utf8buf;

    // step 4: Cleanup and finalization
    cmsSaveProfileToFile(prof_srgb, "srgb_temp.icc");
    cmsSaveProfileToFile(prof_lab, "lab4_temp.icc");
    cmsUnregisterPlugins();

    // API sequence test completed successfully
    return 66;
}