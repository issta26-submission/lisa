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
//<ID> 154
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

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON *author = cJSON_CreateString("unit tester");
    cJSON_AddItemToObject(root, "author", author);

    // step 3: Operate & Validate
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(greet_item);
    cJSON *auth_item = cJSON_GetObjectItem(root, "author");
    const char *auth_val = cJSON_GetStringValue(auth_item);
    cJSON *info = cJSON_CreateObject();
    cJSON *greet_copy = cJSON_CreateString(greet_val);
    cJSON_AddItemToObject(info, "greeting_copy", greet_copy);
    cJSON *auth_copy = cJSON_CreateString(auth_val);
    cJSON_AddItemToObjectCS(info, "author_copy", auth_copy);
    cJSON_AddItemToObject(root, "info", info);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}