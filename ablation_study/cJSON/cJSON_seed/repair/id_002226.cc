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
//<ID> 2226
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
    cJSON_bool add_child_ok = cJSON_AddItemToObject(root, "child", child);
    (void)add_child_ok;
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_bool add_num_ok = cJSON_AddItemToObject(child, "num", num);
    (void)add_num_ok;
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_bool add_greet_ok = cJSON_AddItemToObject(root, "greeting", greeting);
    (void)add_greet_ok;
    cJSON *arr = cJSON_CreateArray();
    cJSON_bool add_arr_ok = cJSON_AddItemToObject(root, "list", arr);
    (void)add_arr_ok;
    cJSON *elem0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, elem0);
    cJSON *elem1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, elem1);

    // step 2: Configure
    cJSON_bool is_obj = cJSON_IsObject(child);
    (void)is_obj;
    cJSON_DeleteItemFromObject(root, "greeting");
    cJSON_DeleteItemFromObjectCaseSensitive(child, "num");

    // step 3: Operate and Validate
    int list_size = cJSON_GetArraySize(arr);
    (void)list_size;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}