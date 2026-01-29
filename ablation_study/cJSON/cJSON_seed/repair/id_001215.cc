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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNumberToObject(config, "threshold", 7.0);
    cJSON_AddStringToObject(config, "name", "alpha");

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(config, 1);
    cJSON_AddItemToArray(arr, dup);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    cJSON *dup_threshold = cJSON_GetObjectItem(dup, "threshold");
    double th_val = cJSON_GetNumberValue(dup_threshold);
    cJSON_AddNumberToObject(root, "detected", th_val + 1.0);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}