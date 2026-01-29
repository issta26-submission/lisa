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
//<ID> 643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *strarray = (cJSON *)0;
    cJSON *extra_str = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_first = (cJSON *)0;
    cJSON *retrieved_last = (cJSON *)0;
    cJSON_bool added_to_obj = 0;
    cJSON_bool added_extra = 0;
    cJSON_bool added_true = 0;
    cJSON_bool is_true_flag = 0;
    int ptr_equal = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    const char * const initial_strings[3] = { "alpha", "beta", "gamma" };
    strarray = cJSON_CreateStringArray(initial_strings, 3);
    extra_str = cJSON_CreateString("delta");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_to_obj = cJSON_AddItemToObject(root, "names", strarray);
    added_extra = cJSON_AddItemToArray(strarray, extra_str);
    added_true = cJSON_AddItemToArray(strarray, true_item);

    // step 4: Operate
    retrieved_first = cJSON_GetArrayItem(strarray, 0);
    retrieved_last = cJSON_GetArrayItem(strarray, 4);
    is_true_flag = cJSON_IsTrue(retrieved_last);
    ptr_equal = (retrieved_last == true_item);

    // step 5: Validate
    (void)added_to_obj;
    (void)added_extra;
    (void)added_true;
    (void)retrieved_first;
    (void)is_true_flag;
    (void)ptr_equal;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}