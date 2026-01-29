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
//<ID> 2153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_bool added_data = cJSON_AddItemToObjectCS(root, "data", data);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello_world");
    cJSON_bool added_greeting = cJSON_AddItemToObjectCS(root, "greeting", greeting);
    cJSON *year = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToObject(root, "year", year);
    cJSON *entry = cJSON_CreateString("entry_one");
    cJSON_AddItemToArray(data, entry);

    // step 3: Operate and Validate
    char *greeting_val = cJSON_GetStringValue(greeting);
    cJSON *data_dup = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "data_copy", data_dup);
    cJSON_bool dup_is_array = cJSON_IsArray(data_dup);
    cJSON *flag = cJSON_CreateBool(dup_is_array);
    cJSON_AddItemToObject(root, "dup_is_array", flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "greeting");
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}