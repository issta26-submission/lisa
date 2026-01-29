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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");

    // step 2: Configure
    cJSON *samples = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "samples", samples);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(samples, n1);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(samples, n2);
    cJSON_AddFalseToObject(settings, "enabled");
    cJSON *threshold = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(settings, "threshold", threshold);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(samples, 1));
    double avg = (v0 + v1) / 2.0;
    cJSON_SetNumberHelper(threshold, avg);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}