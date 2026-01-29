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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *samples = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "samples", samples);

    // step 2: Configure
    double nums[3] = {0.5, 1.5, 2.5};
    cJSON *double_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(data, "double_array", double_array);
    cJSON *tmp_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(tmp_obj, "first", 42.0);
    cJSON *detached_first = cJSON_DetachItemFromObject(tmp_obj, "first");
    cJSON_AddItemToArray(samples, detached_first);
    cJSON *n2 = cJSON_CreateNumber(7.7);
    cJSON_AddItemToArray(samples, n2);
    cJSON_Delete(tmp_obj);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(samples);
    cJSON *first_item = cJSON_GetArrayItem(samples, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddNumberToObject(data, "extracted", first_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}