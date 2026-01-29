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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *item_a = cJSON_CreateString("alpha");
    cJSON *item_b = cJSON_CreateString("beta");
    cJSON *to_replace = cJSON_CreateString("old_value");
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON *compare_with = cJSON_CreateString("gamma");
    cJSON *got_replaced = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_item_a = 0;
    cJSON_bool added_item_b = 0;
    cJSON_bool added_replace = 0;
    cJSON_bool added_compare = 0;
    cJSON_bool replaced_success = 0;
    cJSON_bool cmp_replacement_match = 0;
    cJSON_bool cmp_compare_match = 0;
    int composite_result = 0;

    // step 2: Setup / Configure
    added_item_a = cJSON_AddItemToArray(arr, item_a);
    added_item_b = cJSON_AddItemToArray(arr, item_b);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    added_replace = cJSON_AddItemToObject(root, "replace_me", to_replace);
    added_compare = cJSON_AddItemToObject(root, "compare_with", compare_with);

    // step 3: Operate / Validate
    replaced_success = cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);
    got_replaced = cJSON_GetObjectItem(root, "replace_me");
    cmp_replacement_match = cJSON_Compare(replacement, got_replaced, 1);
    cmp_compare_match = cJSON_Compare(replacement, cJSON_GetObjectItem(root, "compare_with"), 1);
    composite_result = (int)added_arr + (int)added_item_a + (int)added_item_b + (int)added_replace + (int)added_compare + (int)replaced_success + (int)cmp_replacement_match + (int)cmp_compare_match;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)composite_result;
    return 66;
}