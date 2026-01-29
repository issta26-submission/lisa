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
//<ID> 2163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", flag_false);
    cJSON *msg = cJSON_CreateString("operation_complete");
    cJSON_AddItemToObject(root, "message", msg);

    // step 2: Configure
    cJSON *details = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "details", details);
    cJSON *code = cJSON_CreateNumber(200.0);
    cJSON_AddItemToObject(details, "code", code);
    cJSON *note = cJSON_CreateString("no_errors");
    cJSON_AddItemToObject(details, "note", note);

    // step 3: Operate and Validate
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    cJSON_bool status_is_bool = cJSON_IsBool(status_item);
    cJSON *message_item = cJSON_GetObjectItemCaseSensitive(root, "message");
    cJSON_bool message_is_string = cJSON_IsString(message_item);
    cJSON *detached_message = cJSON_DetachItemFromObject(root, "message");
    const char *detached_str = cJSON_GetStringValue(detached_message);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = status_is_bool ? '1' : '0';
    buffer[1] = message_is_string ? '1' : '0';
    buffer[2] = (json && json[0]) ? json[0] : '\0';
    buffer[3] = (detached_str && detached_str[0]) ? detached_str[0] : '\0';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(detached_message);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}