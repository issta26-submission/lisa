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
//<ID> 1611
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *to_remove = cJSON_CreateString("goodbye");
    cJSON_AddItemToObject(root, "to_remove", to_remove);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *s3 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s3);
    cJSON *inner_obj = cJSON_CreateObject();
    cJSON_AddItemToArray(arr, inner_obj);
    cJSON_AddNumberToObject(inner_obj, "val", 7.0);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *scaled = cJSON_CreateNumber(second_val * 10.0);
    cJSON_AddItemToObject(root, "scaled", scaled);
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "to_remove");
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    char *detached_str = cJSON_GetStringValue(detached);
    buffer[0] = detached_str ? detached_str[0] : 'x';
    buffer[1] = (char)('0' + (int)second_val);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}