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
//<ID> 1613
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "alpha");
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *doubled = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToObject(child, "double_val", doubled);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(root, "child");
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *item_after_delete = cJSON_GetArrayItem(arr, 1);
    double after_val = cJSON_GetNumberValue(item_after_delete);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(detached_child, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_str[0];
    buffer[1] = (char)('0' + ((int)second_val % 10));
    buffer[2] = (char)('0' + ((int)after_val % 10));

    // step 4: Cleanup
    cJSON_Delete(detached_child);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}