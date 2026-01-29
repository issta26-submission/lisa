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
//<ID> 1440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *str_ref = cJSON_CreateStringReference("example");
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "null_value", null_item);
    cJSON_AddItemToObjectCS(root, "ref_string", str_ref);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 3: Operate & Validate
    cJSON *dup_str = cJSON_Duplicate(str_ref, 1);
    cJSON_AddItemToObjectCS(child, "dup_string", dup_str);
    cJSON_bool was_false = cJSON_IsFalse(null_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    (void)was_false;

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}