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
//<ID> 2162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *status_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", status_false);
    cJSON *message = cJSON_CreateString("operation_ok");
    cJSON_AddItemToObject(root, "message", message);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *item0 = cJSON_CreateString("first_item");
    cJSON_AddItemToArray(items, item0);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate and Validate
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    cJSON_bool status_is_bool = cJSON_IsBool(status_item);
    cJSON *status_flag = cJSON_CreateBool(status_is_bool);
    cJSON_AddItemToObject(root, "status_is_bool", status_flag);
    cJSON *message_item = cJSON_GetObjectItemCaseSensitive(root, "message");
    cJSON_bool message_is_string = cJSON_IsString(message_item);
    cJSON *message_flag = cJSON_CreateBool(message_is_string);
    cJSON_AddItemToObject(root, "message_is_string", message_flag);
    char *message_text = cJSON_GetStringValue(message_item);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (message_text && message_text[0]) ? message_text[0] : '\0';
    buffer[1] = (json && json[0]) ? json[0] : '\0';
    buffer[2] = (json && json[1]) ? json[1] : '\0';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}