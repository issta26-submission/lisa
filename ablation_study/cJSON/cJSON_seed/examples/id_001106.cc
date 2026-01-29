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
//<ID> 1106
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
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *count_item = cJSON_AddNumberToObject(meta, "count", (double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON_bool is_array = cJSON_IsArray(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *replacement = cJSON_CreateNumber(first_val * 10.0);
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON *after_item = cJSON_GetArrayItem(arr, 0);
    double after_val = cJSON_GetNumberValue(after_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)is_array;
    (void)first_val;
    (void)after_val;
    (void)count_item;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}