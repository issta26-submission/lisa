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
//<ID> 945
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
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *bool_copy = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_InsertItemInArray(arr, 1, bool_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_bool compare_res = cJSON_Compare(bool_item, bool_copy, 1);
    cJSON_AddItemToObject(root, "compare_result", cJSON_CreateBool(compare_res));

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(got_num);
    cJSON_bool second_is_bool = cJSON_IsBool(cJSON_GetArrayItem(arr, 1));
    cJSON_AddNumberToObject(root, "first_value", first_val);
    cJSON_AddItemToObject(root, "is_second_bool", cJSON_CreateBool(second_is_bool));
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(bool_copy);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}