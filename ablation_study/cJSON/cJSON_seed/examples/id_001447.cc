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
//<ID> 1447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *str_ref = cJSON_CreateStringReference("hello");
    cJSON *null_item = cJSON_CreateNull();
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "greeting", str_ref);
    cJSON_AddItemToObjectCS(root, "empty", null_item);
    cJSON_AddItemToObjectCS(root, "ok", false_item);
    cJSON *dup_str = cJSON_Duplicate(str_ref, 1);
    cJSON_AddItemToObjectCS(root, "greeting_dup", dup_str);

    // step 3: Operate & Validate
    cJSON_bool is_false = cJSON_IsFalse(false_item);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *greet_dup_item = cJSON_GetObjectItem(root_dup, "greeting");
    const char *greet_value = cJSON_GetStringValue(greet_dup_item);
    char *printed = cJSON_PrintUnformatted(root_dup);
    (void)is_false;
    (void)greet_value;

    // step 4: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}