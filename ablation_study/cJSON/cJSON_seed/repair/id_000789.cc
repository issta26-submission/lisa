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
//<ID> 789
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
    const char *json_text = "{\"name\":\"parsed_name\",\"value\":123}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON_AddStringToObject(settings, "mode", "active");
    cJSON_AddNumberToObject(settings, "threshold", 3.14);
    cJSON *detached_name = cJSON_DetachItemFromObject(parsed, "name");
    cJSON_AddItemToObject(settings, "imported_name", detached_name);

    // step 3: Operate and Validate
    cJSON *snapshot = cJSON_Duplicate(settings, 1);
    char *out = cJSON_PrintUnformatted(snapshot);
    cJSON_free(out);
    cJSON_Delete(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}