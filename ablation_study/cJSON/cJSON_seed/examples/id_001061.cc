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
//<ID> 1061
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON *flag = cJSON_CreateFalse();
    cJSON *label = cJSON_CreateString("sample");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "status", flag);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_number = cJSON_IsNumber(item0);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(arr, item0, replacement);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool status_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "status"));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}