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
//<ID> 2140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");
    cJSON *child_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(child_obj, "label", "child_label");
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 3: Operate and Validate
    cJSON *fetched_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON_bool is_array = cJSON_IsArray(fetched_items);
    cJSON *first_item = cJSON_GetArrayItem(fetched_items, 0);
    cJSON *dup_first = cJSON_Duplicate(first_item, 1);
    cJSON_AddItemToObject(root, "duplicated_first", dup_first);
    cJSON *fetched_greeting = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    const char *gstr = cJSON_GetStringValue(fetched_greeting);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = gstr[0];
    buffer[1] = '\0';
    (void)is_array; // use variable to avoid unused-variable warnings

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}