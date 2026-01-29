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
//<ID> 1441
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
    cJSON *str_ref = cJSON_CreateStringReference("example");
    cJSON *dup_null = cJSON_Duplicate(null_item, 1);
    cJSON *dup_str = cJSON_Duplicate(str_ref, 1);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON_AddItemToObjectCS(child, "null", null_item);
    cJSON_AddItemToObjectCS(root, "ref", str_ref);
    cJSON_AddItemToObjectCS(root, "dup_null", dup_null);
    cJSON_AddItemToObjectCS(child, "dup_ref", dup_str);

    // step 3: Operate & Validate
    cJSON_bool was_false_original = cJSON_IsFalse(null_item);
    cJSON_bool was_false_dup = cJSON_IsFalse(dup_null);
    (void)was_false_original;
    (void)was_false_dup;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}