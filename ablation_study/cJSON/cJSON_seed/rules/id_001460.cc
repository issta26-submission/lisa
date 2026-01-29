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
//<ID> 1460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *objA = NULL;
    cJSON *num_item = NULL;
    cJSON *false_item = NULL;
    cJSON *null_item = NULL;
    cJSON *replacement = NULL;
    cJSON *dup_item = NULL;
    char *printed = NULL;
    char prebuf[128];
    cJSON_bool is_false = 0;
    cJSON_bool is_null = 0;
    cJSON_bool cmp_eq = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    objA = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", objA);
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(objA, "pi", num_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(objA, "enabled", false_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_item);

    // step 3: Operate / Validate
    is_false = cJSON_IsFalse(false_item);
    is_null = cJSON_IsNull(null_item);
    replacement = cJSON_CreateString("pi_string");
    cJSON_ReplaceItemInObject(objA, "pi", replacement);
    dup_item = cJSON_Duplicate(replacement, 1);
    cJSON_AddItemToObject(root, "copy_of_pi", dup_item);
    cmp_eq = cJSON_Compare(replacement, dup_item, 1);
    printed = cJSON_PrintUnformatted(root);
    memset(prebuf, 0, sizeof(prebuf));
    (void)is_false;
    (void)is_null;
    (void)cmp_eq;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}