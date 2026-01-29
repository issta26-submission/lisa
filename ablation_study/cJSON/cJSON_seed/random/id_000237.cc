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
//<ID> 237
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
    cJSON *item_ref = (cJSON *)0;
    cJSON *item_equal = (cJSON *)0;
    cJSON *item_is_array = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool is_array_check = (cJSON_bool)0;
    cJSON_bool compare_eq = (cJSON_bool)0;
    char *output = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("alpha");
    item_num = cJSON_CreateNumber(3.14);
    item_ref = cJSON_CreateStringReference("alpha");
    cJSON_AddItemToObject(root, "ref", item_ref);

    // step 4: Operate
    add_ok1 = cJSON_AddItemToArray(arr, item_str);
    add_ok2 = cJSON_AddItemToArray(arr, item_num);
    is_array_check = cJSON_IsArray(arr);
    item_is_array = cJSON_CreateBool(is_array_check);
    cJSON_AddItemToObject(root, "is_array", item_is_array);
    compare_eq = cJSON_Compare(item_str, item_ref, (cJSON_bool)1);
    item_equal = cJSON_CreateBool(compare_eq);
    cJSON_AddItemToObject(root, "equal", item_equal);

    // step 5: Validate
    output = cJSON_PrintUnformatted(root);
    cJSON_free(output);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}