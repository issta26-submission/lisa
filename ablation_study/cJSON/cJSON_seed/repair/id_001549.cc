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
//<ID> 1549
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float numbers[3] = {1.1f, 2.2f, 3.3f};
    cJSON *float_array = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON *true_item = cJSON_AddTrueToObject(root, "enabled");
    cJSON *count_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "count", count_item);
    cJSON *flag_item = cJSON_AddBoolToObject(root, "ready", 0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *first_float_item = cJSON_GetArrayItem(float_array, 0);
    double first_float = cJSON_GetNumberValue(first_float_item);
    double count_val = cJSON_GetNumberValue(count_item);
    double combined = count_val + first_float;
    cJSON *combined_num = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(meta, "combined", combined_num);
    cJSON_AddBoolToObject(meta, "configured", 1);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 32);
    memset(scratch, 0, out_len + 32);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (int)count_val);
    scratch[2] = (char)('0' + (int)first_float);
    scratch[3] = (char)('0' + (int)combined);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}