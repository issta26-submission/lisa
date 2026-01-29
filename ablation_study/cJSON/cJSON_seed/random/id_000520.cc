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
//<ID> 520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = 0;
    cJSON *flags = 0;
    cJSON *true_item = 0;
    cJSON *false_item = 0;
    cJSON *greeting_item = 0;
    cJSON *retrieved_flags = 0;
    cJSON *first_flag = 0;
    cJSON_bool added_arr1 = 0;
    cJSON_bool added_arr2 = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool is_first_bool = 0;
    const char *greeting_val = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    flags = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_arr1 = cJSON_AddItemToArray(flags, true_item);
    added_arr2 = cJSON_AddItemToArray(flags, false_item);
    greeting_item = cJSON_AddStringToObject(root, "greeting", "hello");
    added_obj = cJSON_AddItemToObject(root, "flags", flags);

    // step 4: Operate
    retrieved_flags = cJSON_GetObjectItem(root, "flags");
    first_flag = cJSON_GetArrayItem(retrieved_flags, 0);
    is_first_bool = cJSON_IsBool(first_flag);
    greeting_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));

    // step 5: Validate
    (void)is_first_bool;
    (void)greeting_val;
    (void)added_arr1;
    (void)added_arr2;
    (void)added_obj;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}