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
//<ID> 1818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_obj = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    char *arr_str = (char *)0;
    char *child_str = (char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();
    str_item = cJSON_CreateString("world");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToObject(child_obj, "greet", cJSON_CreateString("hello"));
    cJSON_AddItemToObject(root, "child", child_obj);
    cJSON_AddItemToObject(root, "nothing", null_item);
    arr_ref = cJSON_CreateArrayReference((const cJSON *)str_item);
    cJSON_AddItemToObject(root, "refarray", arr_ref);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_arr = cJSON_GetObjectItem(root, "refarray");
    arr_item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    arr_str = cJSON_GetStringValue(arr_item0);
    child_str = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_child, "greet"));

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_child != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(arr_item0 != (cJSON *)0) + (int)(arr_str != (char *)0) + (int)(child_str != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)child_obj;
    (void)str_item;
    (void)null_item;
    (void)arr_ref;
    (void)retrieved_child;
    (void)retrieved_arr;
    (void)arr_item0;
    (void)arr_str;
    (void)child_str;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}