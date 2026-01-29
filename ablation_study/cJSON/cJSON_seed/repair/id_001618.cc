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
//<ID> 1618
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(123.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("third"));
    cJSON_AddNumberToObject(meta, "id", 7.0);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "nested", nested);
    cJSON_AddStringToObject(nested, "note", "ok");

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double value = cJSON_GetNumberValue(second);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    const char *th_str = cJSON_GetStringValue(third);
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    int iv = (int)value;
    buffer[0] = (char)('0' + (iv % 10));
    buffer[1] = th_str[0];
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_Delete(detached_meta);
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *new_first = cJSON_GetArrayItem(arr, 0);
    double new_val = cJSON_GetNumberValue(new_first);
    buffer[2] = (char)('0' + ((int)new_val % 10));

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}