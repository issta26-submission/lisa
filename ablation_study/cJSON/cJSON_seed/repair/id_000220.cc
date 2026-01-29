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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON_AddStringToObject(settings, "name", "example");
    const char *json_literal = "{\"timeout\": 3.5}";
    size_t json_len = sizeof("{\"timeout\": 3.5}") - 1;
    cJSON *parsed = cJSON_ParseWithLength(json_literal, json_len);
    cJSON_AddItemToObject(settings, "parsed", parsed);

    // step 3: Operate and Validate
    cJSON *timeout_item = cJSON_GetObjectItem(parsed, "timeout");
    double timeout_value = cJSON_GetNumberValue(timeout_item);
    cJSON_AddNumberToObject(settings, "timeout_copy", timeout_value);
    cJSON *retrieved_settings = cJSON_GetObjectItem(root, "settings");
    (void)retrieved_settings; // used to show data flow, no branching

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}