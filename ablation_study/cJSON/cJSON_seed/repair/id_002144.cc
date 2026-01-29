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
//<ID> 2144
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
    cJSON *array_entry = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, array_entry);

    // step 2: Configure
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");
    cJSON *greeting_dup = cJSON_Duplicate(greeting, 1);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_dup);
    cJSON_bool items_are_array = cJSON_IsArray(items);
    cJSON_AddBoolToObject(root, "items_are_array", items_are_array);

    // step 3: Operate and Validate
    cJSON *fetched = cJSON_GetObjectItem(root, "greeting_copy");
    const char *gstr = cJSON_GetStringValue(fetched);
    const int buflen = 32;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = gstr[0];
    buffer[1] = (char)('0' + (int)items_are_array);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}