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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float values[3] = { 1.5f, -2.25f, 3.0f };
    cJSON *root = cJSON_CreateObject();
    cJSON *float_array = cJSON_CreateFloatArray(values, 3);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "name", "dataset");
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate and Validate
    char *pretty = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 128, (cJSON_bool)1);
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool is_obj = cJSON_IsObject(retrieved_meta);
    int array_count = cJSON_GetArraySize(float_array);
    cJSON *first_item = cJSON_GetArrayItem(float_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    int summary = (int)array_count + (int)first_val + (int)is_obj;
    (void)summary;

    // step 4: Cleanup
    if (pretty) { cJSON_free(pretty); }
    if (buffered) { cJSON_free(buffered); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}