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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations (Initialize)
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));

    // step 2: Configure
    cmsGetAlarmCodes(alarmCodes);
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(nullProfile);

    // step 3: Operate / Validate
    cmsUInt32Number formatterBytesDefault = cmsFormatterForPCSOfProfile(nullProfile, (cmsUInt32Number)3, (cmsBool)0);
    cmsUInt32Number formatterBytesFloat = cmsFormatterForPCSOfProfile(nullProfile, (cmsUInt32Number)12, (cmsBool)1);
    (void)alarmCodes;
    (void)colorSpace;
    (void)formatterBytesDefault;
    (void)formatterBytesFloat;

    // step 4: Cleanup
    cmsCloseProfile(nullProfile);
    // API sequence test completed successfully
    return 66;
}