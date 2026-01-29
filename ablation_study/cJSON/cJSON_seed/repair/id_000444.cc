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
//<ID> 444
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "user", user);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *name = cJSON_CreateString("bob");
    cJSON_AddItemToObject(user, "name", name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(user, "age", age);
    cJSON_AddItemToArray(items, cJSON_CreateString("first"));
    cJSON_AddItemToArray(items, cJSON_CreateString("second"));
    cJSON_AddItemToArray(items, cJSON_CreateString("third"));

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "active");
    cJSON *retrieved_user = cJSON_GetObjectItem(root, "user");
    cJSON *retrieved_name = cJSON_GetObjectItem(retrieved_user, "name");
    cJSON_bool name_is_str = cJSON_IsString(retrieved_name);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_str);

    // step 3: Operate
    cJSON *retrieved_items = cJSON_GetObjectItem(root, "items");
    cJSON_DeleteItemFromArray(retrieved_items, 0);
    cJSON *second_item = cJSON_GetArrayItem(retrieved_items, 0);
    char *second_value = cJSON_GetStringValue(second_item);
    cJSON_AddStringToObject(root, "picked", second_value);

    // step 4: Validate and Cleanup
    cJSON *picked_item = cJSON_GetObjectItem(root, "picked");
    cJSON_bool picked_is_str = cJSON_IsString(picked_item);
    (void)picked_is_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}