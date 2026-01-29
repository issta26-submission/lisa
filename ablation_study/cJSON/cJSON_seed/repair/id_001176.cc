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
//<ID> 1176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "threshold", 12.5);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial");
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    char *setret = cJSON_SetValuestring(label, "updated");
    (void)setret;

    // step 3: Operate and Validate
    cJSON_bool is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    cJSON *meta_obj = cJSON_GetObjectItem(root, "meta");
    double threshold = cJSON_GetNumberValue(cJSON_GetObjectItem(meta_obj, "threshold"));
    cJSON *enabled_obj = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_obj);
    double final_value = threshold + (double)is_null + (double)enabled_flag;
    cJSON_AddNumberToObject(root, "final", final_value);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}