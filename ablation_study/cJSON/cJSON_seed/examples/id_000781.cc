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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[4] = {1.0, 2.5, 3.75, 4.125};
    cJSON *values_array = cJSON_CreateDoubleArray(nums, 4);
    cJSON *dup_array = cJSON_Duplicate(values_array, 1);
    cJSON *label = cJSON_CreateString("sensor_set");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", values_array);
    cJSON_AddItemToObject(root, "values_copy", dup_array);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int parsed_size = cJSON_GetArraySize(parsed_values);
    cJSON *first_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *new_num = cJSON_CreateNumber(first_val + 0.5);
    cJSON_AddItemToObject(parsed, "first_plus_half", new_num);
    cJSON_bool arrays_equal = cJSON_Compare(values_array, dup_array, 1);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool roots_equal = cJSON_Compare(root, dup_root, 1);
    (void)parsed_size;
    (void)first_val;
    (void)arrays_equal;
    (void)roots_equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}