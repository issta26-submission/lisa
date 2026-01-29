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
//<ID> 207
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
    cJSON_AddItemToObject(root, "child_obj", child);
    cJSON *orig = cJSON_CreateStringReference("original_value");
    cJSON_AddItemToObject(child, "key", orig);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateStringReference("replaced_value");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(child, orig, replacement);
    cJSON *repl_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replaced_flag", repl_flag);

    // step 3: Operate & Validate
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nil", null_item);
    cJSON_bool null_flag = cJSON_IsNull(null_item);
    cJSON *null_flag_num = cJSON_CreateNumber((double)null_flag);
    cJSON_AddItemToObject(root, "null_flag", null_flag_num);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}