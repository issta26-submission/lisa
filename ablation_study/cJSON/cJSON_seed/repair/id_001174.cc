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
//<ID> 1174
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
    cJSON_AddItemToObject(root, "maybe", cJSON_CreateNull());
    cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON *label_ref = cJSON_GetObjectItem(root, "label");
    char *setret = cJSON_SetValuestring(label_ref, "updated");
    (void)setret;

    // step 3: Operate and Validate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(meta_ref, "count"));
    cJSON_bool was_null = cJSON_IsNull(cJSON_GetObjectItem(root, "maybe"));
    double final_value = count_val + (double)was_null;
    cJSON_AddNumberToObject(root, "final", final_value);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}