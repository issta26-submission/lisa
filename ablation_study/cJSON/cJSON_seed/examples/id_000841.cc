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
//<ID> 841
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("Hello, world!");
    cJSON *lang = cJSON_AddStringToObject(root, "language", "C++");

    // step 2: Configure
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "active");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *lang_val = cJSON_GetStringValue(lang);
    char *greet_val = cJSON_GetStringValue(greeting);
    cJSON *echo = cJSON_CreateString(greet_val);
    cJSON_AddItemToObject(root, "echo_greeting", echo);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}