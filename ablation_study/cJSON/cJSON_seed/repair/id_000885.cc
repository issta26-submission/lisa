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
//<ID> 885
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
    cJSON *name_item = cJSON_CreateString("fuzz_target");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 1.23);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold = cJSON_AddNumberToObject(config, "threshold", 10.0);
    cJSON *multiplier = cJSON_AddNumberToObject(config, "multiplier", 2.5);
    cJSON_AddNumberToObject(config, "scaled_threshold", cJSON_GetNumberValue(threshold) * cJSON_GetNumberValue(multiplier));

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "name_copy", name_val);
    cJSON_AddNumberToObject(root, "threshold_tripled", cJSON_GetNumberValue(threshold) * 3.0);
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "meta_ref", name_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}