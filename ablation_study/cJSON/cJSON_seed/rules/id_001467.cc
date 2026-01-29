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
//<ID> 1467
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
    cJSON *item_num = NULL;
    cJSON *replacement = NULL;
    cJSON *dup_num = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    item_str = cJSON_CreateString("initial_value");
    add_ok1 = cJSON_AddItemToObject(root, "name", item_str);
    item_false = cJSON_CreateFalse();
    add_ok2 = cJSON_AddItemToObject(root, "active", item_false);
    item_null = cJSON_CreateNull();
    add_ok3 = cJSON_AddItemToObject(root, "missing", item_null);
    item_num = cJSON_CreateNumber(3.1415);
    add_ok4 = cJSON_AddItemToObject(root, "pi", item_num);
    replacement = cJSON_CreateNumber(42.0);

    // step 3: Operate / Validate
    is_null = cJSON_IsNull(item_null);
    is_false = cJSON_IsFalse(item_false);
    replaced_ok = cJSON_ReplaceItemInObject(root, "name", replacement);
    dup_num = cJSON_Duplicate(item_num, 1);
    cmp_equal = cJSON_Compare(item_num, dup_num, 1);
    (void)add_ok1; (void)add_ok2; (void)add_ok3; (void)add_ok4;
    (void)is_null; (void)is_false; (void)replaced_ok; (void)cmp_equal;

    // step 4: Cleanup
    cJSON_Delete(dup_num);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}