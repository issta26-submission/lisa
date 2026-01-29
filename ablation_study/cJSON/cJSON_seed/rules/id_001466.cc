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
//<ID> 1466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *null_item = NULL;
    cJSON *false_item = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *replacement = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char prebuf[256];
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool replace_ok = 0;
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    null_item = cJSON_CreateNull();
    add_ok1 = cJSON_AddItemToObject(root, "nothing", null_item);
    false_item = cJSON_CreateFalse();
    add_ok2 = cJSON_AddItemToObject(root, "flag", false_item);
    num_item = cJSON_CreateNumber(3.1415);
    add_ok3 = cJSON_AddItemToObject(root, "pi", num_item);
    str_item = cJSON_CreateString("example");
    add_ok4 = cJSON_AddItemToObject(root, "text", str_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateNumber(42.0);
    replace_ok = cJSON_ReplaceItemInObject(root, "pi", replacement);
    dup_root = cJSON_Duplicate(root, 1);
    is_null = cJSON_IsNull(null_item);
    is_false = cJSON_IsFalse(false_item);
    equal = cJSON_Compare(root, dup_root, 1);
    printed = cJSON_PrintUnformatted(root);
    memset(prebuf, 0, sizeof(prebuf));
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)replace_ok;
    (void)is_null;
    (void)is_false;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}