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
//<ID> 935
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *raw = cJSON_CreateRaw("{\"inner\":true}");
    cJSON *pi = cJSON_CreateNumber(3.1415);

    // step 2: Configure
    cJSON_AddItemToArray(arr, name);
    cJSON_AddItemToArray(arr, pi);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate & Validate
    cJSON *raw_dup = cJSON_Duplicate(raw, 1);
    cJSON_AddItemToObject(root, "raw_copy", raw_dup);
    cJSON *items_arr = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items_arr, 0);
    cJSON_bool is_string = cJSON_IsString(first_item);
    cJSON_AddBoolToObject(root, "name_is_string", is_string);
    cJSON *second_item = cJSON_GetArrayItem(items_arr, 1);
    cJSON_SetNumberHelper(second_item, 6.2830);

    // step 4: Cleanup
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup_root);
    cJSON_free(snapshot);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}