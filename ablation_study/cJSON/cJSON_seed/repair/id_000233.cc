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
//<ID> 233
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *n0 = cJSON_CreateNumber(123.0);
    cJSON *s1 = cJSON_CreateString("alpha");

    // step 2: Configure
    cJSON_InsertItemInArray(arr, 0, n0);
    cJSON_InsertItemInArray(arr, 1, s1);
    cJSON_AddItemToObject(meta, "active", flag_false);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "origin", "fuzz_sequence");

    // step 3: Operate
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_active = cJSON_GetObjectItem(retrieved_meta, "active");
    cJSON_bool active_is_false = cJSON_IsFalse(retrieved_active);
    cJSON_AddBoolToObject(root, "active_copy", active_is_false);
    cJSON *retrieved_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(retrieved_items, 0);
    cJSON *second_item = cJSON_GetArrayItem(retrieved_items, 1);
    double first_val = cJSON_GetNumberValue(first_item);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON_AddNumberToObject(root, "first_copy", first_val);
    cJSON_AddStringToObject(root, "second_copy", second_str);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}