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
//<ID> 1169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *flag0 = (cJSON *)0;
    cJSON *flag1 = (cJSON *)0;
    cJSON *enabled = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON_bool is_flag0_bool = 0;
    cJSON_bool is_enabled_bool = 0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array for boolean flags
    root = cJSON_CreateObject();
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Configure - populate the flags array and add a separate boolean member
    flag0 = cJSON_CreateTrue();
    flag1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, flag0);
    cJSON_AddItemToArray(flags, flag1);
    enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 4: Operate - retrieve the flags array and its first element, and retrieve enabled member
    fetched_flags = cJSON_GetObjectItem(root, "flags");
    flag0 = cJSON_GetArrayItem(fetched_flags, 0);
    is_flag0_bool = cJSON_IsBool(flag0);
    enabled = cJSON_GetObjectItem(root, "enabled");
    is_enabled_bool = cJSON_IsBool(enabled);

    // step 5: Validate - compute a simple validation score and serialize the structure
    validation_score = (int)is_flag0_bool + (int)is_enabled_bool + cJSON_GetArraySize(fetched_flags);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}