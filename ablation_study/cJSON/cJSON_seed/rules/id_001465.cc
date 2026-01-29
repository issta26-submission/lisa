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
//<ID> 1465
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
    cJSON *dup_item = NULL;
    char *printed = NULL;
    char prebuf[128];
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool replaced = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    null_item = cJSON_CreateNull();
    false_item = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(3.14159);
    str_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "nil", null_item);
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "name", str_item);

    // step 3: Operate / Validate
    is_null = cJSON_IsNull(null_item);
    is_false = cJSON_IsFalse(false_item);
    replacement = cJSON_CreateNumber(2.71828);
    replaced = cJSON_ReplaceItemInObject(root, "value", replacement);
    dup_item = cJSON_Duplicate(replacement, 1);
    equal = cJSON_Compare(replacement, dup_item, 1);
    printed = cJSON_PrintUnformatted(root);
    memset(prebuf, 0, sizeof(prebuf));
    (void)is_null;
    (void)is_false;
    (void)replaced;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}