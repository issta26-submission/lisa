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
//<ID> 1418
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
    cJSON *values = cJSON_CreateArray();

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.5));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.25));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(5.75));
    cJSON_AddItemToObject(data, "values", values);
    cJSON_AddNumberToObject(data, "count", 3.0);

    // step 3: Operate and Validate
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + cJSON_GetNumberValue(v2);
    cJSON_AddNumberToObject(data, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc((size_t)64);
    memset(tmp, 0, (size_t)64);
    char *buf = (char *)tmp;
    buf[0] = out ? out[0] : 0;
    buf[1] = (char)('0' + ((int)sum % 10));
    buf[2] = (char)('0' + (((int)sum / 10) % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(tmp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}