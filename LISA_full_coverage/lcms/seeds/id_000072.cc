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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Initialize
    cmsDICTentry entryA;
    cmsDICTentry entryB;
    memset(&entryA, 0, sizeof(entryA));
    memset(&entryB, 0, sizeof(entryB));
    entryA.Next = &entryB;
    entryB.Next = NULL;
    const cmsDICTentry * nextEntry = cmsDictNextEntry(&entryA);

    // step 2: Setup
    cmsHPROFILE hFromMem = cmsCreateProfilePlaceholder((cmsContext) NULL);
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    cmsSetProfileVersion(hSRGB, 4.30);
    cmsUInt32Number renderingIntent = cmsGetHeaderCreator(hSRGB);

    // step 3: Operate & Validate
    cmsUInt16Number values[3];
    values[0] = 0;
    values[1] = 32768;
    values[2] = 65535;
    cmsToneCurve * toneCurve = cmsBuildTabulatedToneCurve16((cmsContext) NULL, 3, values);
    cmsBool monotonic = cmsIsToneCurveMonotonic(toneCurve);
    int colorSpaceMapped = (int) cmsFormatterForColorspaceOfProfile(hSRGB, 3, 0);

    // step 4: Cleanup
    toneCurve = NULL;
    hFromMem = NULL;
    hSRGB = NULL;

    (void) nextEntry;
    (void) monotonic;
    (void) colorSpaceMapped;
    (void) renderingIntent;

    // API sequence test completed successfully
    return 66;
}