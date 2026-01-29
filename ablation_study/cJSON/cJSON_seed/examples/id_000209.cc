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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "key", orig);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", maybe_null);

    // step 2: Configure
    cJSON *ref = cJSON_CreateStringReference("referenced_replacement");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, orig, ref);
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_null);
    cJSON *isnull_item = cJSON_CreateBool(is_null_flag);
    cJSON_AddItemToObject(root, "is_null", isnull_item);
    cJSON *replaced_flag_num = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replaced_flag", replaced_flag_num);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}