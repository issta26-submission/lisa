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
//<ID> 891
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.71);

    // step 2: Configure
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate & Validate
    int initial_size = cJSON_GetArraySize(arr);
    cJSON *size_marker = cJSON_CreateNumber((double)initial_size);
    cJSON_AddItemToArray(arr, size_marker);
    char *raw_string = cJSON_GetStringValue(raw);
    cJSON *raw_copy = cJSON_CreateRaw(raw_string ? raw_string : "");
    cJSON_AddItemToArray(arr, raw_copy);
    int final_size = cJSON_GetArraySize(arr);
    cJSON *final_size_num = cJSON_CreateNumber((double)final_size);
    cJSON_AddItemToArray(arr, final_size_num);

    // step 4: Cleanup
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}