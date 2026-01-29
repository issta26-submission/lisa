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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("hello world");
    cJSON *true_item = cJSON_CreateTrue();
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *retrieved_enabled = NULL;
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_greeting = NULL;
    char *greeting_value = NULL;
    cJSON_bool is_enabled = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "greeting", str_item);
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    is_enabled = cJSON_IsTrue(retrieved_enabled);
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_greeting = cJSON_GetObjectItem(retrieved_child, "greeting");
    greeting_value = cJSON_GetStringValue(retrieved_greeting);
    buffer[0] = version[0];
    buffer[1] = (char)('0' + (int)is_enabled);
    buffer[2] = greeting_value[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}