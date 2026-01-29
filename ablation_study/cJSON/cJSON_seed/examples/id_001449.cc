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
//<ID> 1449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *str_ref = cJSON_CreateStringReference("static_string");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON_AddItemToObjectCS(child, "nullval", null_item);
    cJSON_AddItemToObjectCS(root, "sref", str_ref);
    cJSON_AddItemToObjectCS(root, "flag", false_item);

    // step 3: Operate & Validate
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObjectCS(root, "child_dup", dup_child);
    cJSON_bool flag_is_false = cJSON_IsFalse(false_item);
    cJSON_bool null_is_false = cJSON_IsFalse(null_item);
    (void)flag_is_false;
    (void)null_is_false;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}