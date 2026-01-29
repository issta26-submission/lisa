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
//<ID> 2425
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    double v0 = cJSON_GetNumberValue(first_item);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double v1 = cJSON_GetNumberValue(second_item);
    cJSON *sum_item = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[1] = printed2 && printed2[0] ? printed2[0] : '\0';
    buffer[2] = (char)(int)arr_size;
    buffer[3] = (char)(int)first_is_number;
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}