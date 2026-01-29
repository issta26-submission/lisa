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
//<ID> 1417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddNumberToObject(meta, "priority", 5.0);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    char *rendered = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc((size_t)64);
    memset(scratch, 0, (size_t)64);
    scratch[0] = rendered ? rendered[0] : 0;
    scratch[1] = rendered ? rendered[1] : 0;
    scratch[2] = (char)('0' + ((int)v0 % 10));
    scratch[3] = (char)('0' + ((int)v1 % 10));
    scratch[4] = (char)('0' + (size % 10));

    // step 4: Cleanup
    cJSON_free(rendered);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}