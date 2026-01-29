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
//<ID> 647
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
    cJSON *standalone_str = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_bool = 0;
    cJSON_bool is_true = 0;
    int ptr_equal = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    const char *strings_for_array[1] = { "greeting" };
    arr = cJSON_CreateStringArray(strings_for_array, 1);
    standalone_str = cJSON_CreateString("standalone");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_bool = cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "my_array", arr);
    cJSON_AddItemToObject(root, "my_string", standalone_str);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_true = cJSON_IsTrue(retrieved);

    // step 5: Validate
    ptr_equal = (retrieved == true_item);
    (void)added_bool;
    (void)is_true;
    (void)ptr_equal;
    (void)retrieved;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}