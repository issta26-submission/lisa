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
//<ID> 1413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON_AddNumberToObject(metrics, "avg", 12.34);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(metrics, "values", values);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *summary = cJSON_AddObjectToObject(root, "summary");
    cJSON_AddNumberToObject(summary, "max", 30.0);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *json = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    buf[0] = json ? json[0] : '\0';
    buf[1] = (char)('0' + ((int)first_val % 10));
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = buf[0];
    scratch[1] = buf[1];

    // step 4: Cleanup
    cJSON_free(json);
    cJSON_free(buf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}