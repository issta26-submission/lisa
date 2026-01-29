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
//<ID> 1612
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
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddNumberToObject(nested, "id", 123.0);
    cJSON_AddStringToObject(nested, "name", "nestedName");

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, str);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double val = cJSON_GetNumberValue(second);
    cJSON *double_item = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "double_second", double_item);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "nested");
    cJSON_AddItemToArray(array, detached);
    cJSON_DeleteItemFromArray(array, 0);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}