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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *key = cJSON_CreateString("active");
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON *maybe = cJSON_CreateNull();

    // step 2: Configure
    const char *key_name = cJSON_GetStringValue(key);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "key_obj", key);
    cJSON_AddItemToObject(root, "maybe_null", maybe);
    cJSON_AddTrueToObject(root, key_name);

    // step 3: Operate and Validate
    cJSON_bool was_null = cJSON_IsNull(maybe);
    const char *greet_val = cJSON_GetStringValue(greeting);
    cJSON_AddTrueToObject(root, greet_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}