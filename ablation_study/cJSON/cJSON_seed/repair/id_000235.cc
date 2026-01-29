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
//<ID> 235
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *false_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "active", false_flag);
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, item_a);
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(arr, 0, item_b);

    // step 3: Operate / Validate
    cJSON *retrieved_arr = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_item", first_str);
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_active = cJSON_GetObjectItem(retrieved_meta, "active");
    cJSON_bool active_false = cJSON_IsFalse(retrieved_active);
    cJSON_AddBoolToObject(root, "active_copy", active_false);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}