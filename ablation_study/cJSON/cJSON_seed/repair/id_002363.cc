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
//<ID> 2363
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int nums[4] = {1, 2, 3, 4};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *created_num = cJSON_CreateNumber(99.5);
    cJSON_AddItemToObject(root, "created_number", created_num);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "pi", 3.14159);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *retrieved = cJSON_GetObjectItem(root, "created_number");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    cJSON *ints_item = cJSON_GetObjectItem(root, "ints");
    int ints_size = cJSON_GetArraySize(ints_item);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(int)retrieved_val;
    buffer[2] = (char)ints_size;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}