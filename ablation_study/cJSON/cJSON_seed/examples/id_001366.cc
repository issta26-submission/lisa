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
//<ID> 1366
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToObject(root, "count", num);
    cJSON_AddItemToObject(root, "name", str);
    cJSON_AddItemToObject(root, "meta", child);

    // step 2: Configure
    double new_value = cJSON_SetNumberHelper(num, 100.5);
    cJSON_AddBoolToObject(child, "enabled", 0);
    cJSON *echo_num = cJSON_CreateNumber(new_value);
    cJSON_AddItemToObject(child, "echo", echo_num);

    // step 3: Operate & Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    const char *name_val = cJSON_GetStringValue(str);
    double count_val = cJSON_GetNumberValue(num);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *is_object_item = cJSON_CreateNumber((double)is_obj);
    cJSON_AddItemToObject(root, "is_object", is_object_item);
    cJSON *count_echo = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "count_echo", count_echo);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}