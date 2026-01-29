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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *item_str = NULL;
    cJSON *item_false = NULL;
    cJSON *item_null = NULL;
    cJSON *replacement = NULL;
    cJSON *root_copy = NULL;
    cJSON_bool was_replaced = 0;
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool are_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    item_str = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "alpha", item_str);
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "beta", item_false);
    item_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "gamma", item_null);

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(42.0);
    was_replaced = cJSON_ReplaceItemInObject(root, "alpha", replacement);
    is_null = cJSON_IsNull(item_null);
    is_false = cJSON_IsFalse(item_false);
    root_copy = cJSON_Duplicate(root, 1);
    are_equal = cJSON_Compare(root, root_copy, 1);
    (void)was_replaced;
    (void)is_null;
    (void)is_false;
    (void)are_equal;

    // step 4: Cleanup
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}