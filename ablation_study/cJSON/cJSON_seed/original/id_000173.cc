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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float values[3] = {3.14f, 2.718f, 1.0f};
    cJSON *float_array = cJSON_CreateFloatArray(values, 3);
    cJSON *number_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "number", number_item);
    cJSON_AddItemToObject(root, "float_array", float_array);
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_payload", "{\"nested\":123}");

    // step 2: Configure
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_bool raw_is_object = cJSON_IsObject(raw_item);

    // step 3: Operate and Validate
    double extracted = cJSON_GetNumberValue(number_item);
    int summary = (int)extracted + (int)values[0] + (int)root_is_object + (int)raw_is_object;
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)summary;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}