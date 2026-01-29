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
//<ID> 949
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
    cJSON *num0 = cJSON_CreateNumber(1.5);
    cJSON *num1 = cJSON_CreateNumber(2.5);
    cJSON *bool_true = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("sensor-xyz");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, bool_true);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_InsertItemInArray(arr, 1, cJSON_CreateNumber(42.0));

    // step 3: Operate & Validate
    cJSON *item_at_1 = cJSON_GetArrayItem(arr, 1);
    double extracted_value = cJSON_GetNumberValue(item_at_1);
    cJSON *item_at_3 = cJSON_GetArrayItem(arr, 3);
    cJSON_bool third_is_bool = cJSON_IsBool(item_at_3);
    cJSON_bool compare_1_and_2 = cJSON_Compare(item_at_1, num1, 0);
    cJSON_AddItemToObject(root, "extracted_value", cJSON_CreateNumber(extracted_value));
    cJSON_AddItemToObject(root, "third_is_bool", cJSON_CreateBool(third_is_bool));
    cJSON_AddItemToObject(root, "inserted_equals_second", cJSON_CreateBool(compare_1_and_2));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}