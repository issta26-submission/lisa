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
//<ID> 944
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
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(2.0);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_InsertItemInArray(arr, 0, num_b);
    cJSON_AddItemToObject(root, "flag1", flag_true);
    cJSON_AddItemToObject(root, "flag2", flag_false);
    cJSON *dup_a = cJSON_Duplicate(num_a, 1);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    double incremented = first_val + 1.0;
    cJSON *incremented_item = cJSON_CreateNumber(incremented);
    cJSON_InsertItemInArray(arr, 1, incremented_item);
    cJSON *got_flag1 = cJSON_GetObjectItem(root, "flag1");
    cJSON_bool flag1_is_bool = cJSON_IsBool(got_flag1);
    cJSON_AddBoolToObject(root, "flag1_is_bool", flag1_is_bool);
    cJSON_bool equal_check = cJSON_Compare(first, dup_a, 0);
    cJSON_AddBoolToObject(root, "first_equals_dup", equal_check);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}