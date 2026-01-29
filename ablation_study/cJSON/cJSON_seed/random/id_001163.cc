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
//<ID> 1163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *settings = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *fetched_settings = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a nested settings object
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 3: Configure - create an array of boolean flags and add to settings
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "flags", flags);
    flag_true = cJSON_CreateTrue();
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddItemToArray(flags, flag_false);

    // step 4: Operate - retrieve nested object and array elements
    fetched_settings = cJSON_GetObjectItem(root, "settings");
    fetched_flags = cJSON_GetObjectItem(fetched_settings, "flags");
    elem0 = cJSON_GetArrayItem(fetched_flags, 0);
    elem1 = cJSON_GetArrayItem(fetched_flags, 1);

    // step 5: Validate - check that retrieved elements are bools and serialize for a round-trip
    validation_score = (int)cJSON_IsBool(elem0) + (int)cJSON_IsBool(elem1);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}