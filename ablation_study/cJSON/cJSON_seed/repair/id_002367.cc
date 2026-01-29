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
//<ID> 2367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON *manual_num = cJSON_CreateNumber(3.1415);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(int_array, manual_num);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *addedNumItem = cJSON_AddNumberToObject(root, "addedNum", 42.0);
    cJSON *flagItem = cJSON_AddBoolToObject(root, "flag", (cJSON_bool)1);

    // step 3: Operate
    cJSON *ints = cJSON_GetObjectItem(root, "ints");
    int arr_size = cJSON_GetArraySize(ints);
    double num_value = cJSON_GetNumberValue(addedNumItem);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)(int)arr_size;
    buffer[2] = (char)(int)num_value;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}