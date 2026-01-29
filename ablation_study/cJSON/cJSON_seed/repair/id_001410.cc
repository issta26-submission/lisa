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
//<ID> 1410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.5));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.25));
    cJSON_AddNumberToObject(data, "count", 3.0);
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    cJSON *a0 = cJSON_GetArrayItem(arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(arr, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    double sum = v0 + v1 + v2;
    char *scratch = (char *)cJSON_malloc((size_t)64);
    memset(scratch, 0, (size_t)64);
    scratch[0] = json ? json[0] : 0;
    scratch[1] = (char)('0' + ((int)sum % 10));
    scratch[2] = (char)('0' + (((int)sum / 10) % 10));
    scratch[3] = (char)('0' + (((int)sum / 100) % 10));

    // step 4: Cleanup
    cJSON_free(json);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}