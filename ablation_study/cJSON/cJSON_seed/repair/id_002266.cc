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
//<ID> 2266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("deviceX");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *value = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(config, "value", value);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *child_val = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "child_value", child_val);

    // step 2: Configure
    const char *ver_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_dup", dup_child);

    // step 3: Operate and Validate
    cJSON *orig_value = cJSON_GetObjectItem(config, "value");
    cJSON *new_value = cJSON_CreateNumber(456.0);
    cJSON_ReplaceItemViaPointer(config, orig_value, new_value);
    cJSON *copy_of_new = cJSON_Duplicate(new_value, 0);
    cJSON_AddItemToObject(root, "repl_copy", copy_of_new);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    char *name_val = cJSON_GetStringValue(name);
    buffer[1] = (name_val && name_val[0]) ? name_val[0] : '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}