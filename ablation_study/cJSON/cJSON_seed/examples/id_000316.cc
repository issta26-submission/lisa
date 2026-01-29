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
//<ID> 316
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
    int base_ints[3] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(base_ints, 3);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, int_array);
    cJSON_AddItemToObject(root, "mixed", arr);

    // step 3: Operate & Validate
    const cJSON *first = cJSON_GetArrayItem(arr, 0);
    const cJSON *second = cJSON_GetArrayItem(arr, 1);
    const cJSON *third = cJSON_GetArrayItem(arr, 2);
    double first_val = cJSON_GetNumberValue(first);
    const cJSON *third_first = cJSON_GetArrayItem(third, 0);
    double third_first_val = cJSON_GetNumberValue(third_first);
    double sum_val = first_val + third_first_val;
    cJSON *sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_bool second_is_false = cJSON_IsFalse(second);
    cJSON *second_false_item = cJSON_CreateBool(second_is_false);
    cJSON_AddItemToObject(root, "second_is_false", second_false_item);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, (cJSON_bool)1);
    char *printed_unformatted = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObject(root, "printed", cJSON_CreateString(printed_unformatted));

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}