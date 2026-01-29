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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *mixed = cJSON_CreateArray();
    int nums[4] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 4);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(mixed, num_item);
    cJSON_AddItemToArray(mixed, false_item);
    cJSON_AddItemToObject(root, "mixed", mixed);
    cJSON_AddItemToObject(root, "ints", int_arr);

    // step 3: Operate & Validate
    cJSON *first_mixed = cJSON_GetArrayItem(mixed, 0);
    double first_val = cJSON_GetNumberValue(first_mixed);
    cJSON *copied_number = cJSON_CreateNumber(first_val);
    cJSON_AddItemToArray(int_arr, copied_number);
    cJSON *second_int = cJSON_GetArrayItem(int_arr, 1);
    double second_val = cJSON_GetNumberValue(second_int);
    char *printed_unformatted = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, (cJSON_bool)1);

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}