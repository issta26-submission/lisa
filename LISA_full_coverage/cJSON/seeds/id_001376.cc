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
//<ID> 1376
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
    cJSON *child_msg = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "msg", child_msg);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "dup_child", dup_child);
    cJSON *replacement = cJSON_CreateObject();
    cJSON *rep_val = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(replacement, "replaced_value", rep_val);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "child", replacement);

    // step 3: Operate
    cJSON_bool has_dup = cJSON_HasObjectItem(root, "dup_child");
    cJSON *has_dup_num = cJSON_CreateNumber((double)has_dup);
    cJSON_AddItemToObject(root, "has_dup", has_dup_num);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}