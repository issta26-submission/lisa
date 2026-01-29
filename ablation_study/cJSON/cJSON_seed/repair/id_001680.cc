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
//<ID> 1680
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "enabled", enabled_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.23);
    cJSON_AddStringToObject(meta, "note", "initialized");

    // step 3: Operate and Validate
    cJSON *retrieved_name = cJSON_GetObjectItem(root, "name");
    cJSON *retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(retrieved_enabled);
    cJSON_bool name_is_invalid = cJSON_IsInvalid(retrieved_name);
    char *printed = cJSON_Print(root);
    const char *name_str = cJSON_GetStringValue(retrieved_name);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)enabled_is_true);
    buffer[3] = (char)('0' + (int)name_is_invalid);
    (void)buffer;
    (void)name_str;
    (void)retrieved_name;
    (void)retrieved_enabled;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}