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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float numbers[3] = { 1.5f, 2.5f, -3.0f };
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddRawToObject(root, "metadata", "{\"version\":\"1.0\",\"valid\":true}");

    // step 3: Operate and Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *array_item = cJSON_GetObjectItem(root, "floats");
    int array_size = cJSON_GetArraySize(array_item);
    cJSON *first_element = cJSON_GetArrayItem(array_item, 0);
    double first_val = cJSON_GetNumberValue(first_element);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool array_is_obj = cJSON_IsObject(array_item);
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    double summary = count_val + first_val + (double)array_size + (double)root_is_obj + (double)array_is_obj;
    (void)summary;
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}