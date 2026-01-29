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
//<ID> 1610
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "msg", "hello");
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *orig_item = cJSON_GetArrayItem(arr, 1);
    double orig_val = cJSON_GetNumberValue(orig_item);
    cJSON *repl = cJSON_CreateNumber(orig_val * 10.0);
    cJSON_ReplaceItemInArray(arr, 1, repl);

    // step 3: Operate and Validate
    cJSON *checked = cJSON_GetArrayItem(arr, 1);
    double checked_val = cJSON_GetNumberValue(checked);
    char *snapshot = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = snapshot[0];
    buffer[1] = (char)('0' + (int)checked_val % 10);
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(root, "child");
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_Delete(detached_child);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}