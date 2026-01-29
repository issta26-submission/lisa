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
//<ID> 1442
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *str_ref = cJSON_CreateStringReference("example");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "null_value", null_item);
    cJSON_AddItemToObjectCS(root, "flag", false_item);
    cJSON_AddItemToObjectCS(root, "name", str_ref);

    // step 3: Operate & Validate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool flag_is_false = cJSON_IsFalse(false_item);
    cJSON_bool structures_equal = cJSON_Compare(root, dup_root, 1);
    (void)flag_is_false;
    (void)structures_equal;

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}