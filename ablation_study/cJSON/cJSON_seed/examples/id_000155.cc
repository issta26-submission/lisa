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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObjectCS(info, "name", name);
    cJSON *alias = cJSON_CreateString("Ali");
    cJSON_AddItemToObject(root, "alias", alias);
    cJSON *greeting = cJSON_CreateString("Hello, Alice!");
    cJSON_AddItemToObjectCS(root, "greeting", greeting);

    // step 3: Operate & Validate
    cJSON *info_obj = cJSON_GetObjectItem(root, "info");
    cJSON *name_item = cJSON_GetObjectItem(info_obj, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}