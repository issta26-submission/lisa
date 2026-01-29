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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const int nums[] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON *label = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(int_array, 1);
    cJSON_AddItemToObject(root, "numbers_copy", dup_array);
    const char *label_val = cJSON_GetStringValue(label);
    cJSON *label_copy = cJSON_CreateString(label_val);
    cJSON_AddItemToObject(root, "label_copy", label_copy);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(int_array, 0);
    cJSON *second = cJSON_GetArrayItem(int_array, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum_first_two", sum);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}