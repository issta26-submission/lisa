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
//<ID> 1733
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

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(config, "greeting", greeting);
    cJSON *raw_data = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON_AddItemToObject(config, "raw_payload", raw_data);
    cJSON *enabled_node = cJSON_AddBoolToObject(config, "enabled", 1);

    // step 3: Operate and Validate
    char *greeting_val = cJSON_GetStringValue(greeting);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = greeting_val ? greeting_val[0] : 'Y';
    buffer[2] = (char)('0' + cJSON_IsTrue(enabled_node));
    buffer[3] = printed ? (printed[1] ? printed[1] : printed[0]) : 'Z';
    (void)buffer;
    (void)greeting_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}