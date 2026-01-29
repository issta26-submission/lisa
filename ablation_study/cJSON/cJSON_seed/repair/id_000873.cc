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
//<ID> 873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[{\"value\":1.5},{\"value\":2.5}],\"meta\":{\"count\":2}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *first_value = cJSON_GetObjectItem(first, "value");
    double first_v = cJSON_GetNumberValue(first_value);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *second_value = cJSON_GetObjectItem(second, "value");
    double second_v = cJSON_GetNumberValue(second_value);
    cJSON *copied_number1 = cJSON_CreateNumber(0.0);
    double set1 = cJSON_SetNumberHelper(copied_number1, first_v);
    (void)set1;
    cJSON_AddItemToObject(root, "first_copy", copied_number1);
    cJSON *copied_number2 = cJSON_CreateNumber(0.0);
    double set2 = cJSON_SetNumberHelper(copied_number2, second_v);
    (void)set2;
    cJSON_AddItemToObject(root, "second_copy", copied_number2);
    cJSON *sum_array = cJSON_CreateArray();
    cJSON_AddItemToArray(sum_array, cJSON_CreateNumber(first_v + second_v));
    cJSON_AddItemToObject(root, "sum_array", sum_array);

    // step 3: Operate and Validate
    cJSON *retrieved_sum_array = cJSON_GetObjectItem(root, "sum_array");
    cJSON *sum_item = cJSON_GetArrayItem(retrieved_sum_array, 0);
    double sum_val = cJSON_GetNumberValue(sum_item);
    cJSON *sum_verified = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum_verified", sum_verified);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}