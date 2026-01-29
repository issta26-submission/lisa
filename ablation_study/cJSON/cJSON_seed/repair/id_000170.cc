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
//<ID> 170
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
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddFalseToObject(config, "enabled");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *v0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, v0);
    cJSON *v1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, v1);

    // step 3: Operate and Validate
    cJSON_SetNumberHelper(threshold, 2.71);
    double a = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double b = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    double total = a + b + cJSON_GetNumberValue(threshold);
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(stats, "total", total_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}