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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    int nums[3] = {7, 8, 9};
    cJSON *intarr = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToArray(arr, intarr);

    // step 2: Configure
    cJSON *pi_item = cJSON_CreateNumber(3.1415926535);
    cJSON_AddItemToArray(arr, pi_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, false_item);

    // step 3: Operate & Validate
    cJSON *got_intarr = cJSON_GetArrayItem(arr, 0);
    cJSON *got_third = cJSON_GetArrayItem(got_intarr, 2);
    double third_val = cJSON_GetNumberValue(got_third);
    cJSON *got_pi = cJSON_GetArrayItem(arr, 1);
    double pi_val = cJSON_GetNumberValue(got_pi);
    cJSON *sum_item = cJSON_CreateNumber(third_val + pi_val);
    cJSON_AddItemToArray(arr, sum_item);
    char *printed = cJSON_PrintUnformatted(arr);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(arr);
    return 66; // API sequence test completed successfully
}