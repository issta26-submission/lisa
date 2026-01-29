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
//<ID> 1886
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *true_item = cJSON_AddTrueToObject(config, "flag_true");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "flag_false", false_item);
    const double nums[] = {1.1, 2.2, 3.3};
    cJSON *double_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(config, "values", double_array);

    // step 2: Operate
    cJSON *fetched_true = cJSON_GetObjectItem(config, "flag_true");
    cJSON *fetched_false = cJSON_GetObjectItem(config, "flag_false");
    cJSON *fetched_values = cJSON_GetObjectItem(config, "values");
    cJSON_bool is_true = cJSON_IsBool(fetched_true);
    cJSON_bool is_false = cJSON_IsBool(fetched_false);
    int arr_size = cJSON_GetArraySize(fetched_values);
    cJSON *first_value_item = cJSON_GetArrayItem(fetched_values, 0);
    double first_value = cJSON_GetNumberValue(first_value_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (is_true % 10));
    buffer[2] = (char)('0' + (is_false % 10));
    buffer[3] = (char)('0' + (arr_size % 10));
    /* encode a digit from the first numeric value's integer part */
    int first_val_digit = (int)first_value;
    buffer[4] = (char)('0' + (first_val_digit % 10));
    buffer[5] = '\0';

    // step 3: Validate (use results to avoid unused warnings)
    int validate_sum = (int)(buffer[0]) + buffer[1] + buffer[2] + buffer[3] + buffer[4];
    (void)validate_sum;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}