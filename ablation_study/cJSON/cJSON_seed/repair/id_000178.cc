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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *params = cJSON_AddObjectToObject(config, "params");
    cJSON_AddFalseToObject(params, "enabled");

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(values, n2);
    cJSON *n3 = cJSON_CreateNumber(0.0);
    cJSON_AddItemToArray(values, n3);

    // step 3: Operate and Validate
    double a = cJSON_GetNumberValue(n1);
    double b = cJSON_GetNumberValue(n2);
    double sum = a + b;
    cJSON_SetNumberHelper(n3, sum);
    cJSON *summary = cJSON_AddObjectToObject(root, "summary");
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(summary, "sum", sum_item);
    cJSON_AddFalseToObject(summary, "consistent");

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}