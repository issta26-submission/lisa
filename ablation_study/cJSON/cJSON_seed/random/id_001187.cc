#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *profile = (cJSON *)0;
    cJSON *bio = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    char *serialized = (char *)0;
    const char *bio_text = (const char *)0;
    const char *greet_text = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and nested profile
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "greeting", "Hello, world!");
    profile = cJSON_AddObjectToObject(root, "profile");
    cJSON_AddStringToObject(profile, "bio", "Engineer and cJSON user");
    cJSON_AddNumberToObject(profile, "years", 5.0);

    // step 3: Operate - retrieve profile and its fields case-sensitively, extract string values
    profile = cJSON_GetObjectItemCaseSensitive(root, "profile");
    bio = cJSON_GetObjectItemCaseSensitive(profile, "bio");
    greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    bio_text = cJSON_GetStringValue(bio);
    greet_text = cJSON_GetStringValue(greeting);

    // step 4: Validate - serialize unformatted and compute a simple validation score
    serialized = cJSON_PrintUnformatted(root);
    validation_score = (int)(bio_text != (const char *)0) + (int)(greet_text != (const char *)0);

    // step 5: Cleanup - free serialized buffer and delete root which frees attached items
    cJSON_free((void *)serialized);
    cJSON_Delete(root);

    // step 6: Finalization
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}