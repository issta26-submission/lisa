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
//<ID> 1730
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
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *raw = cJSON_CreateRaw("{\"inner\":123}");
    cJSON_AddItemToObject(config, "raw", raw);
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON_AddBoolToObject(root, "active", 0);

    // step 2: Configure
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *note = cJSON_CreateString("payload_note");
    cJSON_AddItemToObject(payload, "note", note);

    // step 3: Operate and Validate
    cJSON *retr_config = cJSON_GetObjectItem(root, "config");
    cJSON *retr_name = cJSON_GetObjectItem(retr_config, "name");
    char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_enabled = cJSON_GetObjectItem(retr_config, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(retr_enabled);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = enabled_is_true ? '1' : '0';
    buffer[2] = name_val ? name_val[0] : 'Z';
    (void)buffer;
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}