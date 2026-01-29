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
//<ID> 1462
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
    cJSON *replacement = NULL;
    cJSON *got_nullable = NULL;
    cJSON *got_enabled = NULL;
    char *printed = NULL;
    char prebuf[128];
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);
    cJSON_AddNumberToObject(root, "count", 7.0);

    // step 3: Operate / Validate
    got_nullable = cJSON_GetObjectItem(root, "nullable");
    is_null = cJSON_IsNull(got_nullable);
    replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(root, "enabled", replacement);
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    is_false = cJSON_IsFalse(got_enabled);
    cmp_equal = cJSON_Compare(replacement, got_enabled, 1);
    printed = cJSON_PrintUnformatted(root);
    memset(prebuf, 0, sizeof(prebuf));
    (void)is_null;
    (void)is_false;
    (void)cmp_equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}