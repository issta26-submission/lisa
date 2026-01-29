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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *flag = cJSON_CreateTrue();
    const char *version = cJSON_Version();
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_greeting = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON_bool flag_is_true = 0;
    const char *greet_str = NULL;
    char buffer[64];
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "version", cJSON_CreateString(version));

    // step 3: Operate / Validate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_greeting = cJSON_GetObjectItem(retrieved_child, "greeting");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_is_true = cJSON_IsTrue(retrieved_flag);
    greet_str = cJSON_GetStringValue(retrieved_greeting);
    buffer[0] = (char)('0' + (int)flag_is_true);
    buffer[1] = greet_str[0];

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}