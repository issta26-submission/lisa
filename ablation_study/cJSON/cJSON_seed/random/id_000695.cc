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
//<ID> 695
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *shared = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_shared = (cJSON *)0;
    cJSON_bool added_shared = 0;
    cJSON_bool added_child = 0;
    cJSON_bool added_ref_to_child = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool is_array = 0;
    cJSON_bool replaced = 0;
    cJSON_bool added_alias = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    shared = cJSON_CreateString("shared-value");
    child = cJSON_CreateObject();
    child_str = cJSON_CreateString("inside-child");
    replacement = cJSON_CreateString("new-child-value");

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    added_shared = cJSON_AddItemToObject(root, "shared", shared);
    cJSON_AddItemToObject(child, "child_key", child_str);
    added_ref_to_child = cJSON_AddItemReferenceToObject(child, "shared_ref", shared);
    added_child = cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    is_array = cJSON_IsArray(retrieved_arr);
    replaced = cJSON_ReplaceItemViaPointer(root, child, replacement);
    retrieved_child = cJSON_GetObjectItem(root, "child");
    added_alias = cJSON_AddItemReferenceToObject(root, "alias_child", retrieved_child);
    retrieved_shared = cJSON_GetObjectItem(root, "shared");

    // step 5: Validate
    (void)added_arr;
    (void)is_array;
    (void)replaced;
    (void)added_shared;
    (void)added_child;
    (void)added_ref_to_child;
    (void)added_alias;
    (void)retrieved_shared;
    (void)retrieved_child;
    (void)retrieved_arr;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}