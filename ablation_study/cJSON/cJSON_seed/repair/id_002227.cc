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
//<ID> 2227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(child, "num", num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greet", str);
    cJSON *a0 = cJSON_CreateString("one");
    cJSON *a1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, a0);
    cJSON_AddItemToArray(arr, a1);

    // step 3: Operate and Validate
    cJSON_bool child_is_obj = cJSON_IsObject(child);
    (void)child_is_obj;
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    (void)got_child;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "arr");
    cJSON_DeleteItemFromObject(root, "child");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}