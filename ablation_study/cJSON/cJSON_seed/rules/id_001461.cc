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
//<ID> 1461
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
    cJSON *cmp_item = NULL;
    char buffer[128];
    cJSON_bool is_false = 0;
    cJSON_bool is_null = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool compare_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("device_name");
    cJSON_AddItemToObject(root, "name", str_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_item);
    replacement = cJSON_CreateNumber(42.0);
    cmp_item = cJSON_CreateNumber(42.0);

    // step 3: Operate / Validate
    replaced_ok = cJSON_ReplaceItemInObject(root, "name", replacement);
    is_false = cJSON_IsFalse(false_item);
    is_null = cJSON_IsNull(null_item);
    compare_equal = cJSON_Compare(replacement, cmp_item, 1);
    memset(buffer, 0, sizeof(buffer));
    (void)replaced_ok;
    (void)is_false;
    (void)is_null;
    (void)compare_equal;

    // step 4: Cleanup
    cJSON_Delete(cmp_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}