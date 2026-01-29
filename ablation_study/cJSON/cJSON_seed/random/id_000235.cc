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
//<ID> 235
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
    cJSON *bool_item = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool add_ok_root = (cJSON_bool)0;
    cJSON_bool add_ok_item1 = (cJSON_bool)0;
    cJSON_bool add_ok_item2 = (cJSON_bool)0;
    cJSON_bool is_array_res = (cJSON_bool)0;
    cJSON_bool compare_res = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_ok_root = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(3.1415);
    add_ok_item1 = cJSON_AddItemToArray(arr, item_str);
    add_ok_item2 = cJSON_AddItemToArray(arr, item_num);

    // step 4: Operate
    is_array_res = cJSON_IsArray(arr);
    bool_item = cJSON_CreateBool(is_array_res);
    cJSON_AddItemToObject(root, "is_items_array", bool_item);
    dup_arr = cJSON_Duplicate(arr, (cJSON_bool)1);
    compare_res = cJSON_Compare(arr, dup_arr, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "items_copy", dup_arr);

    // step 5: Validate
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}