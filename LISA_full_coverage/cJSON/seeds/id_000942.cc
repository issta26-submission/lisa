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
//<ID> 942
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
    cJSON *n0 = cJSON_CreateNumber(10.5);
    cJSON_InsertItemInArray(values, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(20.25);
    cJSON_InsertItemInArray(values, 1, n1);
    cJSON *threshold = cJSON_AddNumberToObject(root, "threshold", 3.14);

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *n2 = cJSON_CreateNumber(second_val * 2.0);
    cJSON_InsertItemInArray(values, 2, n2);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, (char *)buf, 256, 1);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}