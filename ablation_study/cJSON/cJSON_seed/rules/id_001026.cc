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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *settings = NULL;
    cJSON *name = NULL;
    cJSON *enabled = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *dup = NULL;
    cJSON *detached = NULL;
    cJSON *got_settings = NULL;
    cJSON *got_enabled = NULL;
    char *printed = NULL;
    cJSON_bool is_enabled = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    name = cJSON_CreateString("device-alpha");
    cJSON_AddItemToObject(settings, "name", name);
    enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "enabled", enabled);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("entry");
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    detached = cJSON_DetachItemFromObject(dup, "settings");
    cJSON_AddItemToArray(arr, detached);
    got_settings = cJSON_GetObjectItem(root, "settings");
    got_enabled = cJSON_GetObjectItem(got_settings, "enabled");
    is_enabled = cJSON_IsTrue(got_enabled);
    (void)is_enabled;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}