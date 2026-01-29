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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR((cmsContext)NULL);
    cmsMLU * mlu = cmsMLUalloc((cmsContext)NULL, 1);
    char LanguageCode[3] = "en";
    char CountryCode[3]  = "US";
    char ObtainedLanguage[3] = {0};
    char ObtainedCountry[3]  = {0};
    cmsUInt32Number modelSignature = 0x4D4F444C; /* 'MODL' */
    cmsUInt32Number renderingIntent = 0;

    // step 2: Setup MLU entry
    cmsMLUsetUTF8(mlu, LanguageCode, CountryCode, "Manufacturer XYZ");
    cmsMLUgetTranslation(mlu, LanguageCode, CountryCode, ObtainedLanguage, ObtainedCountry);

    // step 3: Core operations on profile
    cmsSetHeaderManufacturer(hProfile, modelSignature);
    renderingIntent = cmsGetHeaderRenderingIntent(hProfile);

    // step 4: Cleanup
    cmsCloseProfile(hProfile);

    (void)mlu;
    (void)LanguageCode;
    (void)CountryCode;
    (void)ObtainedLanguage;
    (void)ObtainedCountry;
    (void)modelSignature;
    (void)renderingIntent;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}