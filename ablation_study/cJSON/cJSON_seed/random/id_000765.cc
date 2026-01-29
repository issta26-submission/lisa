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
//<ID> 765
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
    cJSON *greet = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    cJSON_bool added_child = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_greet = 0;
    cJSON_bool child_is_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    greet = cJSON_CreateString("hello world");

    // step 3: Configure
    added_child = cJSON_AddItemToObject(root, "child", child);
    added_list = cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));
    added_greet = cJSON_AddItemToObject(child, "greeting", greet);

    // step 4: Operate
    retrieved_item = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(retrieved_item);
    child_is_object = cJSON_IsObject(child);

    // step 5: Validate
    validation_score = (retrieved_str != (char *)0) + (int)added_child + (int)added_list + (int)added_greet + (int)child_is_object;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}