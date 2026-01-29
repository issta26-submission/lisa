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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_AddObjectToObject(root, "inner");
    cJSON *flag = cJSON_CreateTrue();
    cJSON *greeting = cJSON_CreateString("hello world");

    // step 2: Configure
    cJSON_AddItemToObject(inner, "enabled", flag);
    cJSON_AddItemToObject(inner, "greeting", greeting);
    cJSON *greeting_ref = cJSON_CreateString(cJSON_GetStringValue(cJSON_GetObjectItem(inner, "greeting")));
    cJSON_AddItemToObject(root, "greeting_copy", greeting_ref);

    // step 3: Operate and Validate
    cJSON *found_inner = cJSON_GetObjectItem(root, "inner");
    cJSON *found_greeting = cJSON_GetObjectItem(found_inner, "greeting");
    const char *val = cJSON_GetStringValue(found_greeting);
    (void)val;
    cJSON *found_copy = cJSON_GetObjectItem(root, "greeting_copy");
    const char *copy_val = cJSON_GetStringValue(found_copy);
    (void)copy_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}