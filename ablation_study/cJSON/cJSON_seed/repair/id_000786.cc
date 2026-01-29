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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\":{\"mode\":\"auto\",\"threshold\":4.5},\"meta\":\"initial\"}";
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON_AddStringToObject(root, "source", "parsed_input");
    cJSON_AddNumberToObject(root, "items", 1.0);

    // step 3: Operate and Validate
    cJSON *config = cJSON_DetachItemFromObject(parsed, "config");
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(config, "status", "merged");
    cJSON_AddNumberToObject(config, "version", 2.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}