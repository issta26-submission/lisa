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
//<ID> 1377
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "original", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greeting", greeting);

    // step 2: Configure
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);
    cJSON *replacement = cJSON_CreateNumber(456.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "original", replacement);

    // step 3: Operate
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *has_child_flag = cJSON_CreateNumber((double)has_child);
    cJSON_AddItemToObject(root, "has_child", has_child_flag);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}