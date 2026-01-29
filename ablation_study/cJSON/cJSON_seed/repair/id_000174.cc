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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *metrics = cJSON_AddObjectToObject(cfg, "metrics");

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_SetNumberHelper(threshold, 15.75);
    cJSON_AddItemToObject(metrics, "threshold", threshold);
    cJSON_AddFalseToObject(metrics, "enabled");
    cJSON *samples = cJSON_CreateArray();
    cJSON *s0 = cJSON_CreateNumber(1.25);
    cJSON *s1 = cJSON_CreateNumber(3.75);
    cJSON_AddItemToArray(samples, s0);
    cJSON_AddItemToArray(samples, s1);
    cJSON_AddItemToObject(cfg, "samples", samples);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 1));
    double total = v0 + v1;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "samples_total", total_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}