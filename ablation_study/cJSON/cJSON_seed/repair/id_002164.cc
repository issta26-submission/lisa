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
//<ID> 2164
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *title = cJSON_CreateString("example_title");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, entry2);
    cJSON *enabled_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", enabled_flag);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 3: Operate and Validate
    cJSON *got_title = cJSON_GetObjectItemCaseSensitive(root, "title");
    cJSON_bool title_is_string = cJSON_IsString(got_title);
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(meta, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON *first_copy_item = cJSON_GetArrayItem(items_copy, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_copy_item);
    cJSON_bool overall_valid = (cJSON_bool)(title_is_string && enabled_is_bool && first_is_string);
    cJSON *valid_node = cJSON_CreateBool(overall_valid);
    cJSON_AddItemToObject(root, "valid", valid_node);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "items_copy");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "pi");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}