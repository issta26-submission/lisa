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
//<ID> 692
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "flags", flags);
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, f1);
    cJSON *f2 = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, f2);
    cJSON *obj_child = cJSON_CreateObject();
    cJSON_AddItemToArray(flags, obj_child);
    cJSON *inner_name = cJSON_CreateString("inner");
    cJSON_AddItemToObject(obj_child, "name", inner_name);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(flags);
    cJSON_AddNumberToObject(settings, "flag_count", (double)arr_size);
    cJSON *first_item = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_object = cJSON_IsObject(first_item);
    cJSON_AddBoolToObject(settings, "first_is_object", first_is_object);
    cJSON *settings_item = cJSON_GetObjectItem(root, "settings");
    cJSON_bool settings_is_object = cJSON_IsObject(settings_item);
    cJSON_AddBoolToObject(root, "settings_is_object", settings_is_object);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}