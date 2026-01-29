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
//<ID> 644
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
    cJSON *true_item = (cJSON *)0;
    cJSON *added_str_item = (cJSON *)0;
    cJSON *retrieved_true = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    const char *initial_strings[1];
    cJSON_bool added_true = 0;
    cJSON_bool added_str = 0;
    cJSON_bool is_true = 0;
    int ptr_equal = 0;
    const char *retrieved_cstr = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    initial_strings[0] = "base";
    arr = cJSON_CreateStringArray(initial_strings, 1);

    // step 3: Configure
    true_item = cJSON_CreateTrue();
    added_str_item = cJSON_CreateString("inserted");
    added_true = cJSON_AddItemToArray(arr, true_item);
    added_str = cJSON_AddItemToArray(arr, added_str_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_true = cJSON_GetArrayItem(arr, 1);
    is_true = cJSON_IsTrue(retrieved_true);
    retrieved_str = cJSON_GetArrayItem(arr, 2);
    retrieved_cstr = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    ptr_equal = (retrieved_str == added_str_item);
    (void)added_true;
    (void)added_str;
    (void)is_true;
    (void)retrieved_cstr;
    (void)ptr_equal;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}