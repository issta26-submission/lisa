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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *value_old = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "value", value_old);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, n3);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *child_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(child, "flag", child_flag);

    // step 2: Configure
    cJSON *value_new = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "value", value_new);
    (void)replaced;
    int before_count = cJSON_GetArraySize(items);

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_item);
    (void)second_invalid;
    cJSON *dup_second = cJSON_Duplicate(second_item, 0);
    cJSON_AddItemToArray(items, dup_second);
    int after_count = cJSON_GetArraySize(items);
    (void)before_count;
    (void)after_count;

    // step 4: Validate and Cleanup
    cJSON_DeleteItemFromArray(items, 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}