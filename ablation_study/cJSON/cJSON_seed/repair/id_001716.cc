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
//<ID> 1716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "user", user);
    cJSON_AddStringToObject(user, "name", "tester");
    cJSON_AddNumberToObject(user, "score", 123.45);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate and Validate
    cJSON *retr_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_arr = cJSON_IsArray(retr_items);
    int arr_size = cJSON_GetArraySize(retr_items);
    cJSON *retr_user = cJSON_GetObjectItem(root, "user");
    cJSON *retr_name = cJSON_GetObjectItem(retr_user, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + (arr_size % 10));
    buffer[3] = (char)('0' + (int)is_arr);
    (void)buffer;
    (void)arr_size;
    (void)is_arr;
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}