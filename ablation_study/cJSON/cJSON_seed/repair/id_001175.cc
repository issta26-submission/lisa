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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *label_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label_item);
    cJSON_AddNullToObject(root, "maybe");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *threshold_item = cJSON_AddNumberToObject(root, "threshold", 10.5);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_null = cJSON_IsNull(maybe_item);
    double threshold_val = cJSON_GetNumberValue(threshold_item);
    cJSON *label_ref = cJSON_GetObjectItem(root, "label");
    char *setret = cJSON_SetValuestring(label_ref, "updated");
    (void)setret;
    double final_val = threshold_val + (double)is_null;
    cJSON_AddNumberToObject(root, "final", final_val);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}