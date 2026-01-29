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
//<ID> 619
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
    cJSON *n0 = cJSON_CreateNumber(3.0);
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON *n2 = cJSON_CreateNumber(11.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "scale", cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(meta, "offset", cJSON_CreateNumber(1.5));

    // step 3: Operate and Validate
    int count_before = cJSON_GetArraySize(numbers);
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    cJSON_AddItemToObject(root, "removed", detached);
    int count_after = cJSON_GetArraySize(numbers);
    cJSON *a0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *a1 = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double average = (v0 + v1) / (double)count_after;
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(average));
    cJSON_AddItemToObject(root, "count_before", cJSON_CreateNumber((double)count_before));
    cJSON_AddItemToObject(root, "count_after", cJSON_CreateNumber((double)count_after));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}