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
//<ID> 1171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "threshold", 42.0);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddStringToObject(root, "status", "initial");
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON_bool optional_is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    char *set_ret = cJSON_SetValuestring(status_item, "updated");
    (void)set_ret;
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "threshold"));
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    double enabled_flag = (double)cJSON_IsTrue(enabled_item);
    double final_value = threshold_val + (double)optional_is_null + enabled_flag;
    cJSON_AddNumberToObject(root, "final", final_value);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}