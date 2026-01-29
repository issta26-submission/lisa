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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[4] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *placeholder = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "to_replace", placeholder);
    cJSON_AddTrueToObject(root, "active");
    cJSON *value_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "value", value_num);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *new_value = cJSON_CreateNumber(first_val + 2.5);
    cJSON_ReplaceItemInObject(root, "value", new_value);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInObject(root, "to_replace", replacement);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    cJSON *final_value = cJSON_GetObjectItem(root, "value");
    double final_val_num = cJSON_GetNumberValue(final_value);
    (void)final_val_num;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}