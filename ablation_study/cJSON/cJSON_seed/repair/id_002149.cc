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
//<ID> 2149
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
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 2: Configure
    cJSON *arr_entry = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(items, arr_entry);
    cJSON *duplicate = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", duplicate);

    // step 3: Operate and Validate
    cJSON_bool dup_is_array = cJSON_IsArray(duplicate);
    cJSON *flag = cJSON_CreateBool(dup_is_array);
    cJSON_AddItemToObject(root, "dup_is_array", flag);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    buffer[1] = json[1];
    buffer[2] = json[2];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}