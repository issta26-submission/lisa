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
//<ID> 2373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(child, "vals", arr);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "shared", arr_ref);
    cJSON_AddItemToObject(root, "child", child);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    cJSON *child_ret = cJSON_GetObjectItem(root, "child");
    cJSON *vals = cJSON_GetObjectItem(child_ret, "vals");
    cJSON_bool has_shared = cJSON_HasObjectItem(root, "shared");
    cJSON_bool is_obj = cJSON_IsObject(child_ret);
    cJSON *first = cJSON_GetArrayItem(vals, 0);
    double first_val = cJSON_GetNumberValue(first);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)first_val;
    buffer[2] = (char)has_shared;
    buffer[3] = (char)is_obj;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}