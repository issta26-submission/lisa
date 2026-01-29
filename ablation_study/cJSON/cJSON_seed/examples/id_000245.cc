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
//<ID> 245
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *summary_count = cJSON_CreateNumber(0.0);
    int count = cJSON_GetArraySize(arr);
    cJSON_SetNumberHelper(summary_count, (double)count);
    cJSON_AddItemToObject(root, "summary_count", summary_count);

    // step 3: Operate & Validate
    cJSON_SetNumberHelper(n1, 3.14159);
    double v1 = cJSON_GetNumberValue(n1);
    double v2 = cJSON_GetNumberValue(n2);
    cJSON *sum = cJSON_CreateNumber(v1 + v2);
    cJSON_AddItemToObject(root, "sum", sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}