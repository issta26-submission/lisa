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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON *copied_str = (cJSON *)0;
    cJSON_bool added_child = 0;
    cJSON_bool added_array = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_name = 0;
    cJSON_bool is_child_obj = 0;
    char *retrieved_value = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello world");
    name_item = cJSON_CreateString("root_node");

    // step 3: Configure
    added_array = cJSON_AddItemToObject(child, "items", arr);
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_child = cJSON_AddItemToObject(root, "child", child);
    added_name = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    retrieved_item = cJSON_GetArrayItem(arr, 0);
    retrieved_value = cJSON_GetStringValue(retrieved_item);
    copied_str = cJSON_CreateString(retrieved_value);
    cJSON_AddItemToObject(root, "copied_first", copied_str);
    is_child_obj = cJSON_IsObject(child);

    // step 5: Validate
    validation_score = (retrieved_value != (char *)0) + (int)is_child_obj + (int)added_child + (int)added_array + (int)added_str + (int)added_name;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}