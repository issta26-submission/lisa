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
//<ID> 713
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
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON *item2 = cJSON_CreateBool(1);
    cJSON_AddItemToArray(items, item0);
    cJSON_AddItemToArray(items, item1);
    cJSON_AddItemToArray(items, item2);
    cJSON_AddItemToObjectCS(root, "items", items);
    cJSON_AddItemToObjectCS(root, "name", cJSON_CreateString("example_name"));
    cJSON_AddItemToObjectCS(root, "enabled", cJSON_CreateBool(0));

    // step 2: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Validate
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON_bool is_array = cJSON_IsArray(parsed_items);
    int count = cJSON_GetArraySize(parsed_items);
    cJSON *first = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *parsed_name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    const char *name_val = cJSON_GetStringValue(parsed_name);
    (void)version;
    (void)is_array;
    (void)count;
    (void)first_str;
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}