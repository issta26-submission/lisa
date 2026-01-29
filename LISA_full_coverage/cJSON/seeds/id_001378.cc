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
//<ID> 1378
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *orig_number = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "replace_me", orig_number);
    cJSON_AddStringToObject(child, "greeting", "hello");

    // step 2: Configure
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    char *printed_root = cJSON_PrintBuffered(root, 128, 1);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replace_me", replacement);

    // step 3: Operate
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *has_item = cJSON_CreateNumber((double)has_child);
    cJSON_AddItemToObject(root, "has_child", has_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed_dup = cJSON_PrintBuffered(dup_root, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_Delete(dup_child);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}