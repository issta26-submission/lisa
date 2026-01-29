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
//<ID> 488
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
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON *s_beta_ref = cJSON_CreateStringReference("beta");
    cJSON_AddItemToArray(arr, s_alpha);
    cJSON_AddItemToArray(arr, s_beta_ref);

    // step 2: Configure
    cJSON *s_gamma = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s_gamma);
    cJSON_AddStringToObject(root, "title", "cJSON-test");
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first_item);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    const char *second_val = cJSON_GetStringValue(second_item);
    char summary[128];
    memset(summary, 0, sizeof(summary));
    sprintf(summary, "first=%s|second=%s", first_val ? first_val : "null", second_val ? second_val : "null");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}