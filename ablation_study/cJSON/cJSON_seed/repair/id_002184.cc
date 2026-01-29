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
//<ID> 2184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n2);
    cJSON *sum_item = cJSON_AddNumberToObject(data, "sum", 0.0);
    cJSON *status_num = cJSON_AddNumberToObject(root, "status", 0.0);
    cJSON *new_status = cJSON_CreateString("ok");
    cJSON_ReplaceItemInObject(root, "status", new_status);

    // step 3: Operate and Validate
    double a = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double b = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double s = a + b;
    cJSON_SetNumberHelper(sum_item, s);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(data, "sum");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}