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
//<ID> 876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "child", child);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(child, "nullable", null_item);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObjectCS(child, "greeting", greeting);
    char *greet_val = cJSON_GetStringValue(greeting);
    const char *ver = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(root, "version", version_item);

    // step 3: Operate
    cJSON *greet_ref = cJSON_CreateStringReference(greet_val);
    cJSON_AddItemToObjectCS(root, "greeting_ref", greet_ref);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON *equal_item = cJSON_CreateBool(equal);
    cJSON_AddItemToObjectCS(root, "equal", equal_item);

    // step 4: Validate & Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}