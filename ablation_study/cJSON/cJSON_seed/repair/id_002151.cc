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
//<ID> 2151
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
    cJSON_AddItemToObjectCS(root, "items", items);
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObjectCS(root, "name", name);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObjectCS(root, "count", num);
    cJSON *arr_entry = cJSON_CreateString("entry1");
    cJSON_AddItemToArray(items, arr_entry);
    cJSON *duplicate = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObjectCS(root, "items_copy", duplicate);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *name_str = (char *)cJSON_GetStringValue(got_name);
    const int buflen = 32;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = name_str[0];
    buffer[1] = '\0';
    char *json = cJSON_PrintUnformatted(root);
    cJSON_bool dup_is_array = cJSON_IsArray(duplicate);
    cJSON *flag = cJSON_CreateBool(dup_is_array);
    cJSON_AddItemToObjectCS(root, "dup_is_array", flag);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "count");

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}