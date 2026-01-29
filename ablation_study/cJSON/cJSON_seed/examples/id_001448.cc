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
//<ID> 1448
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
    cJSON *str_ref = cJSON_CreateStringReference("ref_value");
    cJSON *bool_false = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "null", null_item);
    cJSON_AddItemToObjectCS(root, "ref", str_ref);
    cJSON_AddItemToObjectCS(root, "flag", bool_false);

    // step 3: Operate & Validate
    cJSON *dup_ref = cJSON_Duplicate(str_ref, 0);
    cJSON_AddItemToObjectCS(root, "ref_dup", dup_ref);
    cJSON_bool is_false = cJSON_IsFalse(bool_false);
    cJSON_AddNumberToObject(root, "flag_num", (double)(int)is_false);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}