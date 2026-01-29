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
//<ID> 1221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *greeting = NULL;
    cJSON *new_greeting = NULL;
    cJSON *items_dup = NULL;
    cJSON *first_item = NULL;
    const char *parse_end = NULL;
    const char *first_str = NULL;
    const char *json = "{\"greeting\":\"hello\",\"items\":[\"one\",\"two\",\"three\"]}";
    size_t json_len = sizeof("{\"greeting\":\"hello\",\"items\":[\"one\",\"two\",\"three\"]}") - 1;
    int arr_size = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    items = cJSON_GetObjectItem(parsed, "items");
    items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    greeting = cJSON_GetObjectItem(parsed, "greeting");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(items);
    first_item = cJSON_GetArrayItem(items, 0);
    first_str = cJSON_GetStringValue(first_item);
    new_greeting = cJSON_CreateString("hi");
    replaced = cJSON_ReplaceItemViaPointer(parsed, greeting, new_greeting);
    (void)arr_size;
    (void)first_str;
    (void)replaced;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}