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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = (cmsContext)NULL;
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number)10;
    alarmCodes[15] = (cmsUInt16Number)250;
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    cmsUInt32Number codes[4];
    cmsUInt32Number supported = cmsGetSupportedIntentsTHR(ctx, 4, codes, (char **)NULL);
    cmsFloat64Number tac = (cmsFloat64Number) supported;
    cmsUInt16Number tac16 = (cmsUInt16Number)(tac + 0.5);
    alarmCodes[1] = tac16;
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    (void) io;
    (void) hProfile;
    (void) tac;
    (void) tac16;
    return 66;
}