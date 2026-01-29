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
//<ID> 334
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
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *flag = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_greeting = NULL;
    cJSON_bool flag_is_true = 0;
    const char *greeting_text = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "enabled", flag);
    cJSON_AddItemToObject(child, "greeting", greeting);
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_flag = cJSON_GetObjectItem(retrieved_child, "enabled");
    retrieved_greeting = cJSON_GetObjectItem(retrieved_child, "greeting");
    flag_is_true = cJSON_IsTrue(retrieved_flag);
    greeting_text = cJSON_GetStringValue(retrieved_greeting);
    buffer[0] = (char)('0' + (int)flag_is_true);
    buffer[1] = version[0];
    buffer[2] = greeting_text[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}