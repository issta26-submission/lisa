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
//<ID> 897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *raw1 = cJSON_CreateRaw("\"embedded-raw\"");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, raw1);

    // step 3: Operate & Validate
    int initial_size = cJSON_GetArraySize(arr);
    cJSON *size_marker = cJSON_CreateNumber((double)initial_size);
    cJSON_AddItemToArray(arr, size_marker);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    const char *raw_value = cJSON_GetStringValue(second_item);
    cJSON *raw_copy = cJSON_CreateRaw(raw_value);
    cJSON_AddItemToArray(arr, raw_copy);
    int final_size = cJSON_GetArraySize(arr);
    cJSON *final_size_item = cJSON_CreateNumber((double)final_size);
    cJSON_AddItemToArray(arr, final_size_item);

    // step 4: Cleanup
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}