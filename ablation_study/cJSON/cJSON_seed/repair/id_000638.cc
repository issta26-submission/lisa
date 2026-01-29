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
//<ID> 638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", version);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    double updated_n0 = cJSON_SetNumberHelper(n0, 15.5);
    cJSON_AddNumberToObject(root, "n0_updated", updated_n0);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(values, n1, replacement);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double sum = v0 + v1;
    double avg = sum / (double)count;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "avg", avg);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}