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
//<ID> 1179
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
    cJSON_AddNumberToObject(config, "threshold", 10.0);
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial");
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON_bool was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    char *setret = cJSON_SetValuestring(label, "updated");
    (void)setret;
    cJSON_AddBoolToObject(root, "active", 1);
    double thresh = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "threshold"));
    double final_val = thresh + (double)was_null;
    cJSON_AddNumberToObject(root, "final", final_val);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}