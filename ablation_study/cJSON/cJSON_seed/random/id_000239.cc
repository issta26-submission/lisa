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
//<ID> 239
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
    cJSON *dup_str = (cJSON *)0;
    cJSON *cmp_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool add_ok_obj = (cJSON_bool)0;
    cJSON_bool add_ok_arr1 = (cJSON_bool)0;
    cJSON_bool add_ok_arr2 = (cJSON_bool)0;
    cJSON_bool is_array_check = (cJSON_bool)0;
    cJSON_bool compare_equal = (cJSON_bool)0;
    int array_size = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_ok_obj = cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("example");
    item_num = cJSON_CreateNumber(2025.0);
    add_ok_arr1 = cJSON_AddItemToArray(arr, item_str);
    add_ok_arr2 = cJSON_AddItemToArray(arr, item_num);

    // step 4: Operate
    is_array_check = cJSON_IsArray(arr);
    dup_str = cJSON_Duplicate(item_str, (cJSON_bool)1);
    compare_equal = cJSON_Compare(item_str, dup_str, (cJSON_bool)1);
    cJSON_Delete(dup_str);
    cmp_item = cJSON_CreateBool(compare_equal);
    cJSON_AddItemToObject(root, "items_equal", cmp_item);

    // step 5: Validate
    array_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "count", count_item);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}