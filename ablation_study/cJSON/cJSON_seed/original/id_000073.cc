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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float numbers[3];
    numbers[0] = 1.5f;
    numbers[1] = 2.5f;
    numbers[2] = 3.5f;
    cJSON *values = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "example");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    char *pretty = cJSON_Print(root);
    char *buf_pretty = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *vals = cJSON_GetObjectItem(root, "values");
    int count = cJSON_GetArraySize(vals);
    cJSON *first_item = cJSON_GetArrayItem(vals, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);
    int summary = (int)count + (int)root_is_obj + (int)first_val + (int)meta_is_obj;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(buf_pretty);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}