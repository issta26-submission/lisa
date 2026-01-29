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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double values_arr[2] = {3.14, 2.71};
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(values_arr[0]);
    cJSON *num1 = cJSON_CreateNumber(values_arr[1]);
    cJSON_AddItemToArray(values, num0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToObject(config, "values", values);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *summary = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 2: Configure
    cJSON *values_ref = cJSON_GetObjectItem(config, "values");
    cJSON *val_item0 = cJSON_GetArrayItem(values_ref, 0);
    cJSON *val_item1 = cJSON_GetArrayItem(values_ref, 1);
    double v0 = cJSON_GetNumberValue(val_item0);
    double v1 = cJSON_GetNumberValue(val_item1);
    double total = v0 + v1;
    cJSON_SetNumberHelper(summary, total);

    // step 3: Operate
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, summary);
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON *detached_values = cJSON_DetachItemFromObjectCaseSensitive(config, "values");
    cJSON_AddItemToObject(root, "detached_values", detached_values);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON *summary_after = cJSON_GetObjectItem(root, "summary");
    double verify_total = cJSON_GetNumberValue(summary_after);
    (void)verify_total;
    cJSON_DeleteItemFromObject(root, "config");
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}