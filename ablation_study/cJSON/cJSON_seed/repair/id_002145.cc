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
//<ID> 2145
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

    // step 2: Configure
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");
    char *updated = cJSON_SetValuestring(greeting, "hello_modified");
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "inner", "value");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num0);

    // step 3: Operate and Validate
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);
    cJSON_bool is_arr = cJSON_IsArray(items);
    cJSON *flag_num = cJSON_CreateNumber((double)is_arr);
    cJSON_AddItemToObject(root, "is_array_flag", flag_num);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = updated ? updated[0] : (printed ? printed[0] : '\0');
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}