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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float values[3] = {1.1f, 2.2f, 3.3f};
    cJSON *floatArray = cJSON_CreateFloatArray(values, 3);
    cJSON_AddItemToObject(root, "floats", floatArray);
    cJSON *number_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "num", number_item);
    cJSON_AddRawToObject(root, "raw_json", "{\"nested\":true}");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "status", "ok");

    // step 3: Operate and Validate
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *num_retrieved = cJSON_GetObjectItem(root, "num");
    double num_val = cJSON_GetNumberValue(num_retrieved);
    cJSON *floats_retrieved = cJSON_GetObjectItem(root, "floats");
    cJSON_bool floats_is_obj = cJSON_IsObject(floats_retrieved);
    cJSON *first_float_item = cJSON_GetArrayItem(floats_retrieved, 0);
    double first_float_val = cJSON_GetNumberValue(first_float_item);
    int summary = (int)num_val + (int)first_float_val + (int)floats_is_obj + root->type + meta->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}