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
//<ID> 612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON *n3 = cJSON_CreateNumber(40.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    cJSON_AddItemToObject(root, "removed", detached);
    int count = cJSON_GetArraySize(values);
    cJSON *a0 = cJSON_GetArrayItem(values, 0);
    cJSON *a1 = cJSON_GetArrayItem(values, 1);
    cJSON *a2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(average));
    cJSON_AddItemToObject(root, "original_count", cJSON_CreateNumber(4.0));
    cJSON_AddItemToObject(root, "current_count", cJSON_CreateNumber((double)count));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}