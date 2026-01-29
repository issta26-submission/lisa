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
//<ID> 2361
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
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", int_array);
    cJSON *nitem = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(int_array, nitem);
    cJSON *obj_number = cJSON_AddNumberToObject(root, "answer", 7.0);
    cJSON *obj_bool = cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(int_array);
    cJSON *first_item = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *ans_item = cJSON_GetObjectItem(root, "answer");
    double ans_val = cJSON_GetNumberValue(ans_item);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)first_val;
    buffer[2] = (char)(int)arr_size;
    buffer[3] = (char)(int)ans_val;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}