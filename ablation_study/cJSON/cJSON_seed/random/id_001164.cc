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
//<ID> 1164
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
    cJSON *bool_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_settings = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON_bool is_bool_elem = 0;
    int validation_score = 0;

    // step 2: Initialize - create root and nested objects/array
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "flags", flags);

    // step 3: Configure - populate array and add an extra member to root
    bool_item = cJSON_CreateBool(1);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(flags, bool_item);
    cJSON_AddItemToArray(flags, num_item);
    cJSON_AddItemToObject(root, "release", cJSON_CreateString("stable"));

    // step 4: Operate - retrieve nested object, its array, and the first element
    fetched_settings = cJSON_GetObjectItem(root, "settings");
    fetched_flags = cJSON_GetObjectItem(fetched_settings, "flags");
    fetched_elem0 = cJSON_GetArrayItem(fetched_flags, 0);

    // step 5: Validate - check type and compute a small validation score
    is_bool_elem = cJSON_IsBool(fetched_elem0);
    validation_score = (int)(fetched_settings != (cJSON *)0) + (int)(fetched_flags != (cJSON *)0) + (int)is_bool_elem;
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}