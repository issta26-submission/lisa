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
//<ID> 1739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *name_node = cJSON_CreateString("example");
    cJSON_AddItemToObject(data, "name", name_node);
    cJSON *raw_node = cJSON_CreateRaw("\"rawvalue\"");
    cJSON_AddItemToObject(data, "raw", raw_node);
    cJSON *enabled_node = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "note", note);

    // step 3: Operate and Validate
    cJSON *retr_data = cJSON_GetObjectItem(root, "data");
    cJSON *retr_name = cJSON_GetObjectItem(retr_data, "name");
    cJSON *retr_raw = cJSON_GetObjectItem(retr_data, "raw");
    const char *name_val = cJSON_GetStringValue(retr_name);
    const char *raw_val = cJSON_GetStringValue(retr_raw);
    cJSON *retr_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(retr_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = raw_val ? raw_val[0] : 'Z';
    buffer[3] = (char)('0' + (int)is_enabled);
    (void)buffer;
    (void)name_val;
    (void)raw_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}