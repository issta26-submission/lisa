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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateNumber(2.5);
    cJSON *expected = cJSON_CreateObject();
    cJSON *exp_arr = cJSON_CreateArray();
    cJSON *count_item = NULL;
    cJSON *double_item = NULL;
    cJSON *second_elem = NULL;
    char *printed = NULL;
    double second_val = 0.0;
    int array_size = 0;
    cJSON_bool equal_flag = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    count_item = cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(expected, "data", exp_arr);
    cJSON_AddItemToArray(exp_arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(exp_arr, cJSON_CreateNumber(2.5));
    cJSON_AddNumberToObject(expected, "count", (double)cJSON_GetArraySize(exp_arr));

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(arr);
    second_elem = cJSON_GetArrayItem(arr, 1);
    second_val = cJSON_GetNumberValue(second_elem);
    double_item = cJSON_AddNumberToObject(root, "double_second", second_val * 2.0);
    equal_flag = cJSON_Compare(root, expected, 1);
    cJSON_AddBoolToObject(root, "equal", equal_flag);
    printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(expected);
    // API sequence test completed successfully
    return 66;
}