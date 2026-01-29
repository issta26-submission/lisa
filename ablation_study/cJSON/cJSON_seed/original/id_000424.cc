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
//<ID> 424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30, 40};
    cJSON *ints = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *placeholder = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "replace_me", placeholder);
    cJSON *initial_count = cJSON_CreateNumber(4.0);
    cJSON_AddItemToObject(root, "count", initial_count);

    // step 2: Configure
    cJSON *replacement_number = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInObject(root, "replace_me", replacement_number);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 128, 1);
    cJSON *replaced = cJSON_GetObjectItem(root, "replace_me");
    double replaced_val = cJSON_GetNumberValue(replaced);
    cJSON *scaled = cJSON_CreateNumber(replaced_val * 1.5);
    cJSON_AddItemToObject(root, "scaled_replace", scaled);
    cJSON *ints_copy = cJSON_Duplicate(ints, 1);
    cJSON_AddItemToObject(root, "ints_copy", ints_copy);

    // step 4: Validate and Cleanup
    int size_of_ints = cJSON_GetArraySize(ints);
    cJSON *size_num = cJSON_CreateNumber((double)size_of_ints);
    cJSON_AddItemToObject(root, "ints_size", size_num);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}