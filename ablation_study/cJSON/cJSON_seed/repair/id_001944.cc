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
//<ID> 1944
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);
    cJSON *second = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, second);
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    cJSON_bool is_array = cJSON_IsArray(arr);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_greeting = cJSON_GetObjectItem(child, "greeting");
    char *greet_val = cJSON_GetStringValue(got_greeting);
    buffer[0] = greet_val ? greet_val[0] : 'X';
    buffer[1] = (char)('0' + (was_false % 10));
    buffer[2] = (char)('0' + (is_array % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}