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
//<ID> 853
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
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    const char *json_text = "{\"module\":{\"enabled\":true,\"threshold\":7}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *module = cJSON_GetObjectItem(parsed, "module");
    cJSON *module_ref = cJSON_CreateObjectReference(module);
    cJSON_AddItemToObject(root, "imported_module", module_ref);

    // step 3: Operate and Validate
    cJSON *pi_lookup = cJSON_GetObjectItem(config, "pi");
    double pi_val = cJSON_GetNumberValue(pi_lookup);
    cJSON_AddNumberToObject(root, "pi_copy", pi_val);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}