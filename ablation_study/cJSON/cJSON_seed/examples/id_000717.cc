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
//<ID> 717
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
    cJSON *list = cJSON_CreateArray();
    cJSON *item1 = cJSON_CreateString("item_one");
    cJSON *item2 = cJSON_CreateString("item_two");
    cJSON *active_flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToArray(list, item2);
    cJSON_AddItemToObjectCS(root, "list", list);
    cJSON_AddItemToObjectCS(root, "active", active_flag);

    // step 2: Configure
    cJSON *title = cJSON_CreateString("example_title");
    cJSON_AddItemToObjectCS(root, "title", title);
    cJSON_bool list_is_array = cJSON_IsArray(list);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_list = cJSON_GetObjectItemCaseSensitive(parsed, "list");
    cJSON_bool parsed_list_is_array = cJSON_IsArray(parsed_list);
    cJSON *parsed_title = cJSON_GetObjectItemCaseSensitive(parsed, "title");
    const char *title_value = cJSON_GetStringValue(parsed_title);
    cJSON *parsed_active = cJSON_GetObjectItemCaseSensitive(parsed, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(parsed_active);
    cJSON *first_item = cJSON_GetArrayItem(parsed_list, 0);
    const char *first_value = cJSON_GetStringValue(first_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    (void)list_is_array;
    (void)parsed_list_is_array;
    (void)title_value;
    (void)active_is_bool;
    (void)first_value;
    // API sequence test completed successfully
    return 66;
}