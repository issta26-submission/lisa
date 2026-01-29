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
//<ID> 947
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
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON *num2 = cJSON_CreateNumber(3.5);
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_InsertItemInArray(arr, 1, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate & Validate
    cJSON *got1 = cJSON_GetArrayItem(arr, 1);
    double v1 = cJSON_GetNumberValue(got1);
    cJSON *got2 = cJSON_GetArrayItem(arr, 2);
    double v2 = cJSON_GetNumberValue(got2);
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_bool equal = cJSON_Compare(arr, dup_arr, 1);
    cJSON_bool flag_is_bool = cJSON_IsBool(cJSON_GetObjectItem(root, "flag"));
    cJSON_AddNumberToObject(root, "sum", v1 + v2);
    cJSON_AddBoolToObject(root, "array_equal_dup", equal);
    cJSON_AddBoolToObject(root, "flag_is_bool", flag_is_bool);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}