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
//<ID> 2138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, null_item);
    cJSON *str_in_array = cJSON_CreateString("first");
    cJSON_AddItemToArray(array, str_in_array);

    // step 2: Configure
    cJSON_SetValuestring(greeting, "hi_there");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num);

    // step 3: Operate and Validate
    int before = cJSON_GetArraySize(array);
    cJSON_DeleteItemFromArray(array, 0);
    int after = cJSON_GetArraySize(array);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    const char *s = cJSON_GetStringValue(item0);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = s[0];
    buffer[1] = (char)('0' + (after % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}