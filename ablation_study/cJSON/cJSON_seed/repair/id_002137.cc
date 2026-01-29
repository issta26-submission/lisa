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
//<ID> 2137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *a1 = cJSON_CreateString("first");
    cJSON *a2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, a1);
    cJSON_AddItemToArray(arr, a2);
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, n);
    char *set_ret = cJSON_SetValuestring(greeting, "world");

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = first_str[0];
    buffer[1] = set_ret[0];
    buffer[2] = '\0';
    cJSON_DeleteItemFromArray(arr, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}