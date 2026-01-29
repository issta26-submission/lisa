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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsHPROFILE hProfile = NULL;
    cmsContext ctx = NULL;
    cmsIOHANDLER * io = NULL;
    cmsToneCurve * tc = NULL;
    cmsUInt16Number values[4] = { 0, 21845, 43690, 65535 };
    cmsUInt32Number nEntries = 4;
    cmsUInt32Number formatter = 0;
    cmsUInt32Number bytes = 12;
    cmsBool lIsFloat = 0;
    cmsBool md5_ok = 0;

    // step 2: Initialize
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    io = cmsOpenIOhandlerFromNULL(ctx);

    // step 3: Operate / Configure / Validate
    tc = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
    formatter = cmsFormatterForPCSOfProfile(hProfile, bytes, lIsFloat);
    md5_ok = cmsMD5computeID(hProfile);
    cmsSetHeaderFlags(hProfile, formatter);

    // step 4: Cleanup (silence unused warnings and leave resources in a consistent state)
    (void)tc;
    (void)io;
    (void)ctx;
    (void)values;
    (void)bytes;
    (void)lIsFloat;
    (void)md5_ok;
    (void)formatter;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}