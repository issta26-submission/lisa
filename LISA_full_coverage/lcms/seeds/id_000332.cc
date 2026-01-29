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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE nullProfile = NULL;
    cmsUInt16Number alarms[16];
    cmsUInt32Number formatterResult = 0;
    cmsUInt32Number colorSpaceCode = 0;

    // step 2: Setup / Configure
    nullProfile = cmsCreateNULLProfileTHR(ctx);
    memset(alarms, 0, sizeof(alarms));
    cmsGetAlarmCodes(alarms);

    // step 3: Operate / Validate
    colorSpaceCode = (cmsUInt32Number) cmsGetColorSpace(nullProfile);
    formatterResult = cmsFormatterForPCSOfProfile(nullProfile, (cmsUInt32Number)8, (cmsBool)0);
    (void)colorSpaceCode;
    (void)formatterResult;
    (void)alarms;

    // step 4: Cleanup
    cmsCloseProfile(nullProfile);
    // API sequence test completed successfully
    return 66;
}