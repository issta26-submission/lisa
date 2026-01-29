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
//<ID> 2228
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "value", num);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON_bool child_is_obj = cJSON_IsObject(got_child);
    (void)child_is_obj;
    int arr_size = cJSON_GetArraySize(arr);
    (void)arr_size;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_DeleteItemFromObject(child, "value");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "arr");

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}