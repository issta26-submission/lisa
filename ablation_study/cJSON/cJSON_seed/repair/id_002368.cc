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
//<ID> 2368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[3] = {10, 20, 30};
    cJSON *root = cJSON_CreateObject();
    cJSON *intArray = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "int_list", intArray);

    // step 2: Configure
    cJSON *extraNum = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(intArray, extraNum);
    cJSON_AddNumberToObject(root, "pi_approx", 3.14159);
    cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(intArray);
    cJSON *first_item = cJSON_GetArrayItem(intArray, 0);
    double first_val = cJSON_GetNumberValue(first_item);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)first_val;
    buffer[2] = (char)arr_size;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}