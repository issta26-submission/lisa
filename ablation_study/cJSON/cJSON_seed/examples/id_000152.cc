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
//<ID> 152
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
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *greeting = cJSON_CreateString("Hello, Alice!");
    cJSON_AddItemToObjectCS(root, "greeting_cs", greeting);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root, "age", age);
    cJSON *version = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "cjson_version", version);

    // step 3: Operate & Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *greet_item = cJSON_GetObjectItem(root, "greeting_cs");
    const char *greet_str = cJSON_GetStringValue(greet_item);
    cJSON *greet_ref = cJSON_CreateString(greet_str);
    cJSON_AddItemToObject(root, "greeting_copy", greet_ref);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}