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
//<ID> 941
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
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(-3.25);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON_InsertItemInArray(arr, 1, n2);
    cJSON_InsertItemInArray(arr, 2, n3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "threshold", 7.75);

    // step 2: Configure
    cJSON *n_extra = cJSON_CreateNumber(99.99);
    cJSON_InsertItemInArray(arr, 1, n_extra);
    cJSON *item_at_1 = cJSON_GetArrayItem(arr, 1);
    double val = cJSON_GetNumberValue(item_at_1);
    cJSON_AddNumberToObject(root, "picked", val);

    // step 3: Operate
    void *buf = cJSON_malloc(512);
    memset(buf, 0, 512);
    cJSON_PrintPreallocated(root, (char *)buf, 512, 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}