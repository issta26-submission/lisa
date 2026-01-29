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
//<ID> 594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *active = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(4.0));
    int size_after = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    cJSON *a3 = cJSON_GetArrayItem(items, 3);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2) + cJSON_GetNumberValue(a3);
    double average = sum / (double)size_after;
    double multiplier = 1.0 + (double)cJSON_IsTrue(active);
    double adjusted = average * multiplier;
    cJSON_AddNumberToObject(root, "average", average);
    cJSON_AddNumberToObject(root, "adjusted_average", adjusted);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}