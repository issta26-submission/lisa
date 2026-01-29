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
//<ID> 1463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *null_item = NULL;
    cJSON *false_item = NULL;
    cJSON *replacement = NULL;
    cJSON *dup_num = NULL;
    char *printed = NULL;
    char prebuf[128];
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool cmp_equal = 0;
    cJSON_bool replace_ok = 0;
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(root, "name", name_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    replacement = cJSON_CreateNumber(3.14);
    replace_ok = cJSON_ReplaceItemInObject(root, "name", replacement);
    dup_num = cJSON_CreateNumber(3.14);

    // step 3: Operate / Validate
    is_null = cJSON_IsNull(null_item);
    is_false = cJSON_IsFalse(false_item);
    cmp_equal = cJSON_Compare(replacement, dup_num, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)replace_ok;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_num);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}