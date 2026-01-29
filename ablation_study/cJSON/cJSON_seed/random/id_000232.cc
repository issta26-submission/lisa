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
//<ID> 232
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
    cJSON *count_item = (cJSON *)0;
    cJSON *dup_arr = (cJSON *)0;
    char *out_text = (char *)0;
    int array_size = 0;
    cJSON_bool add_ok_obj = (cJSON_bool)0;
    cJSON_bool add_ok_arr1 = (cJSON_bool)0;
    cJSON_bool add_ok_arr2 = (cJSON_bool)0;
    cJSON_bool is_array_flag = (cJSON_bool)0;
    cJSON_bool arrays_equal = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_ok_obj = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(3.14);
    add_ok_arr1 = cJSON_AddItemToArray(arr, item_str);
    add_ok_arr2 = cJSON_AddItemToArray(arr, item_num);
    is_array_flag = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "items_are_array", is_array_flag);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "count", count_item);
    dup_arr = cJSON_Duplicate(arr, (cJSON_bool)1);
    arrays_equal = cJSON_Compare(arr, dup_arr, (cJSON_bool)1);
    cJSON_AddBoolToObject(root, "arrays_equal", arrays_equal);

    // step 5: Validate
    out_text = cJSON_PrintUnformatted(root);
    cJSON_free(out_text);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_arr);

    // API sequence test completed successfully
    return 66;
}