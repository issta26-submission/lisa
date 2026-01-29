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
//<ID> 710
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");
    cJSON *flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, str2);
    cJSON_AddItemToObjectCS(root, "items", items);
    cJSON_AddItemToObjectCS(root, "enabled", flag);
    cJSON_AddItemToObjectCS(root, "title", cJSON_CreateString("demo"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON_bool items_are_array = cJSON_IsArray(parsed_items);
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON *parsed_enabled = cJSON_GetObjectItemCaseSensitive(parsed, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(parsed_enabled);
    (void)version;
    (void)items_are_array;
    (void)first_value;
    (void)enabled_is_bool;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}