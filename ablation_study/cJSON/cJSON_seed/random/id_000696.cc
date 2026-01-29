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
//<ID> 696
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
    cJSON *holder = (cJSON *)0;
    cJSON *arr_item = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_alias = (cJSON *)0;
    cJSON *replacement_arr = (cJSON *)0;
    cJSON *replacement_item = (cJSON *)0;
    cJSON_bool was_array_before = 0;
    cJSON_bool was_array_alias = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_holder = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool replaced = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    arr_item = cJSON_CreateString("original");
    holder = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToArray(arr, arr_item);
    added_list = cJSON_AddItemToObject(root, "list", arr);
    added_ref = cJSON_AddItemReferenceToObject(holder, "alias", arr);
    added_holder = cJSON_AddItemToObject(root, "holder", holder);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    was_array_before = cJSON_IsArray(retrieved_list);
    replacement_arr = cJSON_CreateArray();
    replacement_item = cJSON_CreateString("replacement");
    cJSON_AddItemToArray(replacement_arr, replacement_item);
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_list, replacement_arr);

    // step 5: Validate
    retrieved_alias = cJSON_GetObjectItem(holder, "alias");
    was_array_alias = cJSON_IsArray(retrieved_alias);
    (void)was_array_before;
    (void)was_array_alias;
    (void)added_list;
    (void)added_holder;
    (void)added_ref;
    (void)replaced;
    (void)retrieved_list;
    (void)retrieved_alias;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}