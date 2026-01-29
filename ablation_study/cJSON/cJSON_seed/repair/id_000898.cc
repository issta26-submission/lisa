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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, true_item);
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n1);

    // step 2: Configure
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double total = v0 + v1;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(values);
    cJSON *snapshot = cJSON_Duplicate(root, 1);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_bool printed = cJSON_PrintPreallocated(snapshot, buffer, 512, 1);
    cJSON_bool equal = cJSON_Compare(root, snapshot, 1);
    (void)printed;
    (void)equal;
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}