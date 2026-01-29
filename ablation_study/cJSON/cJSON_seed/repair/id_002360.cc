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
//<ID> 2360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(99.5);
    cJSON_AddItemToArray(int_array, extra_num);
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(int_array);
    cJSON *third = cJSON_GetArrayItem(int_array, 2);
    double third_val = cJSON_GetNumberValue(third);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)third_val;
    buffer[2] = (char)(int)pi_val;
    buffer[3] = (char)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}