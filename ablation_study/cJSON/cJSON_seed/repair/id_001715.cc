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
//<ID> 1715
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(7.25));
    cJSON *third_str_item = cJSON_CreateString("inner_value");
    cJSON_AddItemToArray(items, third_str_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "ok");
    cJSON_AddItemToArray(items, meta);

    // step 3: Operate and Validate
    cJSON_bool is_arr = cJSON_IsArray(items);
    cJSON *retr_items = cJSON_GetObjectItem(root, "items");
    cJSON *retr_flag = cJSON_GetObjectItem(root, "flag");
    cJSON *third = cJSON_GetArrayItem(items, 2);
    const char *third_val = cJSON_GetStringValue(third);
    char *buffer = (char *)cJSON_malloc(32);
    memset(buffer, 0, 32);
    buffer[0] = (char)('0' + (int)is_arr);
    buffer[1] = third_val ? third_val[0] : 'X';
    const char *flag_val = cJSON_GetStringValue(retr_flag);
    buffer[2] = flag_val ? flag_val[0] : 'F';
    (void)retr_items;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}