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
//<ID> 2223
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "inner_num", num);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON *arr_el = cJSON_CreateString("elem");
    cJSON_AddItemToArray(arr, arr_el);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *num_in_child = cJSON_GetObjectItem(child, "inner_num");
    double prev_val = cJSON_GetNumberValue(num_in_child);
    (void)prev_val;
    double set_res = cJSON_SetNumberHelper(num_in_child, 456.789);
    (void)set_res;
    cJSON *flag = cJSON_AddTrueToObject(root, "flag");
    (void)flag;

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON_bool is_obj = cJSON_IsObject(got_child);
    (void)is_obj;
    cJSON *got_greet = cJSON_GetObjectItem(root, "greeting");
    const char *gstr = cJSON_GetStringValue(got_greet);
    (void)gstr;
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

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "child");
    cJSON_DeleteItemFromObject(root, "greeting");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}