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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"config\": {\"alpha\": 3, \"beta\": 4}, \"value\": 42}";
    cJSON *root = cJSON_Parse(json);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double original_value = cJSON_GetNumberValue(value_item);

    // step 2: Configure
    cJSON *generated = cJSON_AddObjectToObject(root, "generated");
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(generated, "config_ref", config_ref);
    cJSON_AddNumberToObject(generated, "original_value", original_value);

    // step 3: Operate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    (void)has_cfg;
    char *out = cJSON_Print(root);

    // step 4: Validate and Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}