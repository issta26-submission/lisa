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
//<ID> 234
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
    cJSON *meta = (cJSON *)0;
    cJSON *status = (cJSON *)0;
    cJSON *root_copy = (cJSON *)0;
    cJSON *compare_bool = (cJSON *)0;
    cJSON *cmp_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool is_array_flag = (cJSON_bool)0;
    cJSON_bool compare_res = (cJSON_bool)0;
    int arr_size = 0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, item_str);
    cJSON_AddItemToArray(arr, item_num);
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(meta, "status", status);

    // step 4: Operate
    is_array_flag = cJSON_IsArray(arr);
    compare_bool = cJSON_CreateBool(is_array_flag);
    cJSON_AddItemToObject(root, "items_is_array", compare_bool);
    root_copy = cJSON_Duplicate(root, (cJSON_bool)1);
    compare_res = cJSON_Compare(root, root_copy, (cJSON_bool)1);
    cmp_item = cJSON_CreateBool(compare_res);
    cJSON_AddItemToObject(root, "root_equal_copy", cmp_item);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}