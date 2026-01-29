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
//<ID> 1468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *false_item = NULL;
    cJSON *null_item = NULL;
    cJSON *replacement = NULL;
    cJSON *dup_null = NULL;
    char *printed = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool replace_ok = 0;
    cJSON_bool is_false = 0;
    cJSON_bool is_null = 0;
    cJSON_bool cmp_eq = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    add_ok1 = cJSON_AddItemToObject(root, "greeting", str_item);
    false_item = cJSON_CreateFalse();
    add_ok2 = cJSON_AddItemToObject(root, "flag", false_item);
    null_item = cJSON_CreateNull();
    add_ok3 = cJSON_AddItemToObject(root, "maybe", null_item);
    replacement = cJSON_CreateString("hi");

    // step 3: Operate / Validate
    replace_ok = cJSON_ReplaceItemInObject(root, "greeting", replacement);
    is_false = cJSON_IsFalse(false_item);
    is_null = cJSON_IsNull(null_item);
    dup_null = cJSON_CreateNull();
    cmp_eq = cJSON_Compare(null_item, dup_null, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)replace_ok;
    (void)is_false;
    (void)is_null;
    (void)cmp_eq;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_null);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}