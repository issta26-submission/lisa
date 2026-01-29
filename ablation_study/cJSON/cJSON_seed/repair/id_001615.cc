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
//<ID> 1615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "nums", array);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "label", "alpha");
    cJSON_AddItemToObject(root, "childObj", child);

    // step 2: Configure
    cJSON *extra = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *double_item = cJSON_CreateNumber(second_val * 2.5);
    cJSON_AddItemToObject(root, "double_second", double_item);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    const char *label_before = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(child, "label"));
    buffer[0] = label_before ? label_before[0] : '\0';
    buffer[1] = (char)('0' + (int)second_val % 10);
    cJSON_DeleteItemFromArray(array, 0);
    cJSON *detached_child = cJSON_DetachItemFromObjectCaseSensitive(root, "childObj");
    const char *label_detached = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(detached_child, "label"));
    buffer[2] = label_detached ? label_detached[0] : '\0';

    // step 4: Cleanup
    cJSON_Delete(detached_child);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}