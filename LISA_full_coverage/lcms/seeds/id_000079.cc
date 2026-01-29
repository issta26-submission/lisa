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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsDICTentry entry1;
    cmsDICTentry entry2;
    unsigned char profileMem[128];
    cmsUInt16Number tcValues[4];
    cmsToneCurve * tone = NULL;
    const cmsDICTentry * next = NULL;
    const cmsDICTentry * next2 = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsBool isMonotonic = 0;
    int cs = 0;
    int combined = 0;

    // step 2: Setup
    memset(&entry1, 0, sizeof(entry1));
    memset(&entry2, 0, sizeof(entry2));
    entry1.Next = &entry2;
    entry2.Next = NULL;

    memset(profileMem, 0, sizeof(profileMem));
    profileMem[0] = 0x00;
    profileMem[1] = 0x00;
    profileMem[2] = 0x00;
    profileMem[3] = 0x80; // size = 128 (big-endian)
    profileMem[36] = 'a';
    profileMem[37] = 'c';
    profileMem[38] = 's';
    profileMem[39] = 'p';

    tcValues[0] = (cmsUInt16Number)0;
    tcValues[1] = (cmsUInt16Number)16384;
    tcValues[2] = (cmsUInt16Number)49152;
    tcValues[3] = (cmsUInt16Number)65535;

    tone = cmsBuildTabulatedToneCurve16((cmsContext)NULL, 4U, tcValues);
    hProfile = cmsOpenProfileFromMem(profileMem, (cmsUInt32Number)sizeof(profileMem));

    // step 3: Operate and Validate
    next = cmsDictNextEntry(&entry1);
    next2 = cmsDictNextEntry(next);
    isMonotonic = cmsIsToneCurveMonotonic(tone);
    cs = _cmsLCMScolorSpace((cmsColorSpaceSignature)0);
    combined = cs + (int)isMonotonic;

    // step 4: Cleanup
    cmsFreeToneCurve(tone);
    hProfile = NULL;
    (void)next;
    (void)next2;
    (void)isMonotonic;
    (void)cs;
    (void)combined;

    return 66;
    // API sequence test completed successfully
}