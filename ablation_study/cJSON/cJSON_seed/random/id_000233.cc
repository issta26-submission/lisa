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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item_str = (cJSON *)0;
    cJSON *item_num = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    cJSON *isarray_item = (cJSON *)0;
    cJSON *equal_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool is_array_check = (cJSON_bool)0;
    cJSON_bool compare_result = (cJSON_bool)0;
    char *json_out = (char *)0;
    int array_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, item_str);
    cJSON_AddItemToArray(arr, item_num);

    // step 4: Operate
    is_array_check = cJSON_IsArray(arr);
    isarray_item = cJSON_CreateBool(is_array_check);
    cJSON_AddItemToObject(root, "is_array", isarray_item);
    dup_arr = cJSON_Duplicate(arr, (cJSON_bool)1);
    compare_result = cJSON_Compare(arr, dup_arr, (cJSON_bool)1);
    equal_item = cJSON_CreateBool(compare_result);
    cJSON_AddItemToObject(root, "items_equal_dup", equal_item);

    // step 5: Validate
    array_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "count", count_item);
    json_out = cJSON_PrintUnformatted(root);
    cJSON_free(json_out);

    // step 6: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}