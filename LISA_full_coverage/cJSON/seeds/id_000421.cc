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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints1[] = {1, 2, 3, 4};
    cJSON *int_array = cJSON_CreateIntArray(ints1, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *old_value = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "replace_me", old_value);

    // step 2: Configure
    cJSON *replacement_obj = cJSON_CreateObject();
    cJSON *answer_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(replacement_obj, "answer", answer_num);
    cJSON_ReplaceItemInObject(root, "replace_me", replacement_obj);

    // step 3: Operate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *pi_scaled = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "pi_scaled", pi_scaled);
    int ints2[] = {10, 20};
    cJSON *more_ints = cJSON_CreateIntArray(ints2, 2);
    cJSON_AddItemToObject(root, "more_ints", more_ints);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    cJSON *repl = cJSON_GetObjectItem(root, "replace_me");
    cJSON *ans = cJSON_GetObjectItem(repl, "answer");
    double ans_val = cJSON_GetNumberValue(ans);
    cJSON *inc_ans = cJSON_CreateNumber(ans_val + 1.0);
    cJSON_AddItemToObject(root, "incremented_answer", inc_ans);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}