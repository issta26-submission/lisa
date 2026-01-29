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
//<ID> 2427
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
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *single = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "single", single);

    // step 2: Configure
    int array_size = cJSON_GetArraySize(arr);
    char meta[8];
    memset(meta, 0, sizeof(meta));
    meta[0] = (char)array_size;

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_item);
    double first_value = cJSON_GetNumberValue(first_item);
    buffer[1] = (char)(int)first_is_number;
    buffer[2] = (char)(int)first_value;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}