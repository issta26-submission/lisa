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
//<ID> 1168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *fetched_config = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *fetched_enabled = (cJSON *)0;
    cJSON *fetched_list = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON_bool enabled_is_bool = 0;
    cJSON_bool elem0_is_bool = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, nested objects and an array, and attach them
    root = cJSON_CreateObject();
    config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    flags = cJSON_CreateObject();
    cJSON_AddItemToObject(config, "flags", flags);
    cJSON_AddItemToObject(flags, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToObject(flags, "visible", cJSON_CreateFalse());
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Configure - populate the array with a boolean and a number
    item0 = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateNumber(2025.0);
    cJSON_AddItemToArray(arr, item1);

    // step 4: Operate - retrieve nested members and array element using required APIs
    fetched_config = cJSON_GetObjectItem(root, "config");
    fetched_flags = cJSON_GetObjectItem(fetched_config, "flags");
    fetched_enabled = cJSON_GetObjectItem(fetched_flags, "enabled");
    enabled_is_bool = cJSON_IsBool(fetched_enabled);
    fetched_list = cJSON_GetObjectItem(root, "list");
    fetched_elem0 = cJSON_GetArrayItem(fetched_list, 0);
    elem0_is_bool = cJSON_IsBool(fetched_elem0);

    // step 5: Validate - compute a simple score, serialize and free the printed buffer
    validation_score = (int)(fetched_config != (cJSON *)0) + (int)enabled_is_bool + (int)elem0_is_bool;
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}