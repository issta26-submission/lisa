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
//<ID> 610
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "scale", cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(meta, "description", cJSON_CreateString("sample set"));

    // step 3: Operate and Validate
    int initial_count = cJSON_GetArraySize(numbers);
    cJSON *removed_item = cJSON_DetachItemFromArray(numbers, 1);
    int after_detach_count = cJSON_GetArraySize(numbers);
    cJSON *a0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *a1 = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    cJSON *scale = cJSON_GetObjectItem(meta, "scale");
    double s = cJSON_GetNumberValue(scale);
    double scaled_sum = (v0 + v1) * s;
    cJSON_AddItemToObject(root, "scaled_sum", cJSON_CreateNumber(scaled_sum));
    cJSON_AddItemToObject(meta, "removed", removed_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}