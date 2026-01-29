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
//<ID> 946
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_InsertItemInArray(arr, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_InsertItemInArray(arr, 1, n1);
    cJSON *n2 = cJSON_CreateNumber(3.25);
    cJSON_InsertItemInArray(arr, 2, n2);

    // step 3: Operate
    cJSON *g0 = cJSON_GetArrayItem(arr, 0);
    cJSON *g1 = cJSON_GetArrayItem(arr, 1);
    cJSON *g2 = cJSON_GetArrayItem(arr, 2);
    double sum = cJSON_GetNumberValue(g0) + cJSON_GetNumberValue(g1) + cJSON_GetNumberValue(g2);
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Validate & Cleanup
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, (char *)buf, 256, 1);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}