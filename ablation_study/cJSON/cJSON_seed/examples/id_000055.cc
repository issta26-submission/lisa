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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(array);
    cJSON *a0 = cJSON_GetArrayItem(array, 0);
    cJSON *a1 = cJSON_GetArrayItem(array, 1);
    cJSON *a2 = cJSON_GetArrayItem(array, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double sum = v0 + v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *printed = cJSON_PrintUnformatted(root);
    (void)size;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}