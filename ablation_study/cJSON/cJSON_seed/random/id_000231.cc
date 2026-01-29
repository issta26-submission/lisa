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
//<ID> 231
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
    cJSON *flag_item = (cJSON *)0;
    cJSON *equal_flag = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_array_check = (cJSON_bool)0;
    cJSON_bool compare_result = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(3.14);
    add_ok1 = cJSON_AddItemToArray(arr, item_str);
    add_ok2 = cJSON_AddItemToArray(arr, item_num);
    is_array_check = cJSON_IsArray(arr);
    flag_item = cJSON_CreateBool(is_array_check);
    cJSON_AddItemToObject(root, "items_is_array", flag_item);

    // step 4: Operate
    dup_arr = cJSON_Duplicate(arr, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "duplicate", dup_arr);
    compare_result = cJSON_Compare(arr, dup_arr, (cJSON_bool)1);
    equal_flag = cJSON_CreateBool(compare_result);
    add_ok3 = cJSON_AddItemToObject(root, "arrays_equal", equal_flag);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}