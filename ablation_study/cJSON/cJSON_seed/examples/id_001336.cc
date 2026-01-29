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
//<ID> 1336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *label = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "flag", false_item);
    cJSON_AddItemToObject(child, "raw", raw_item);
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Operate
    cJSON_bool has_label = cJSON_HasObjectItem(root, "label");
    cJSON *has_num = cJSON_CreateNumber((double)has_label);
    cJSON_AddItemToObject(root, "has_label", has_num);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    const char *label_val = cJSON_GetStringValue(label);
    cJSON *label_copy = cJSON_CreateString(label_val);
    cJSON_AddItemToObject(root, "label_copy", label_copy);

    // step 3: Validate
    cJSON_bool child_has_flag = cJSON_HasObjectItem(child, "flag");
    cJSON_AddNumberToObject(root, "child_has_flag", (double)child_has_flag);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}