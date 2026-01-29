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
//<ID> 1619
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
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *toDetach = cJSON_CreateObject();
    cJSON_AddItemToObject(toDetach, "inner", cJSON_CreateString("inner_value"));
    cJSON_AddItemToObject(root, "toDetach", toDetach);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "root_val", 3.14);
    cJSON *newnum = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(arr, newnum);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double v = cJSON_GetNumberValue(second);
    cJSON *doubled = cJSON_CreateNumber(v * 2.0);
    cJSON_AddItemToArray(arr, doubled);
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *det = cJSON_DetachItemFromObjectCaseSensitive(root, "toDetach");
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    cJSON *inner = cJSON_GetObjectItemCaseSensitive(det, "inner");
    const char *s = cJSON_GetStringValue(inner);
    buffer[0] = s[0];
    buffer[1] = (char)('0' + ((int)v % 10));

    // step 4: Cleanup
    cJSON_Delete(det);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}