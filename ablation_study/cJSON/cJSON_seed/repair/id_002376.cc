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
//<ID> 2376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(inner, "values", arr);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "arr_ref", arr_ref);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];

    // step 3: Operate
    cJSON *inner_found = cJSON_GetObjectItem(root, "inner");
    cJSON_bool is_obj = cJSON_IsObject(inner_found);
    cJSON *values_found = cJSON_GetObjectItem(inner_found, "values");
    cJSON *first = cJSON_GetArrayItem(values_found, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "arr_ref");
    cJSON *ref_item = cJSON_GetObjectItem(root, "arr_ref");
    cJSON_bool ref_is_obj = cJSON_IsObject(ref_item);
    buffer[1] = (char)(int)first_val;
    buffer[2] = (char)(int)(is_obj + has_ref + ref_is_obj);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}