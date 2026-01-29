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
//<ID> 1700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "test_case");
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    float thresholds_vals[3];
    thresholds_vals[0] = 1.5f;
    thresholds_vals[1] = 2.5f;
    thresholds_vals[2] = 3.75f;
    cJSON *thresholds = cJSON_CreateFloatArray(thresholds_vals, 3);
    cJSON_AddItemToObject(config, "thresholds", thresholds);
    cJSON *enabled = cJSON_CreateBool((cJSON_bool)1);
    cJSON_AddItemToObject(config, "enabled", enabled);
    cJSON_AddFalseToObject(config, "disabled");
    cJSON_AddNumberToObject(root, "version", 1.2);

    // step 2: Configure
    cJSON_AddStringToObject(root, "description", "float-array-and-bools");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "samples", 3.0);

    // step 3: Operate and Validate
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_config = cJSON_GetObjectItem(root, "config");
    cJSON *retr_thresholds = cJSON_GetObjectItem(retr_config, "thresholds");
    cJSON *first_item = cJSON_GetArrayItem(retr_thresholds, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    char *out = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = name_val ? name_val[0] : 'X';
    buffer[1] = out ? out[0] : 'Y';
    buffer[2] = (char)('0' + (int)first_val);
    buffer[3] = (char)('0' + (int)cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "samples")));
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}