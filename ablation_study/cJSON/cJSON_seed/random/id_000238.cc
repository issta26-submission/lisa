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
//<ID> 238
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
    cJSON *item_dup = (cJSON *)0;
    cJSON *arr_dup = (cJSON *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool cmp_result = (cJSON_bool)0;
    char *printed = (char *)0;
    int array_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(42.0);
    item_dup = cJSON_Duplicate(item_str, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("example"));

    // step 4: Operate
    cJSON_AddItemToArray(arr, item_str);
    cJSON_AddItemToArray(arr, item_num);
    cJSON_AddItemToArray(arr, item_dup);
    is_arr = cJSON_IsArray(arr);
    cJSON_AddItemToObject(root, "is_array", cJSON_CreateBool(is_arr));
    arr_dup = cJSON_Duplicate(arr, (cJSON_bool)1);
    cmp_result = cJSON_Compare(arr, arr_dup, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "arrays_equal", cJSON_CreateBool(cmp_result));

    // step 5: Validate
    array_size = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber((double)array_size));
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(arr_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}