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
//<ID> 173
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
    cJSON *flag = cJSON_AddFalseToObject(config, "enabled");

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToObject(config, "values", values);

    // step 3: Operate and Validate
    double updated = cJSON_SetNumberHelper(n0, 3.14);
    double v0 = cJSON_GetNumberValue(n0);
    double v1 = cJSON_GetNumberValue(n1);
    cJSON *sum_item = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}