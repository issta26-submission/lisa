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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[] = {10, 20, 30};
    cJSON *arr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    const char external_text[] = "{\"external\":123,\"flag\":true}";
    cJSON *external = cJSON_ParseWithLength(external_text, sizeof(external_text) - 1);
    cJSON_AddItemToObject(root, "external", external);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool first_eq_second = cJSON_Compare(first, second, 1);
    cJSON_AddBoolToObject(meta, "first_equals_second", first_eq_second);
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *new_second = cJSON_GetArrayItem(arr, 1);
    double new_second_val = cJSON_GetNumberValue(new_second);
    cJSON_AddNumberToObject(meta, "new_second_value", new_second_val);

    // step 4: Cleanup and final validation
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(meta, "snapshot", snapshot);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}