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
//<ID> 785
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
    cJSON_AddStringToObject(settings, "mode", "auto");
    cJSON_AddNumberToObject(settings, "threshold", 3.14);
    const char *json_blob = "{\"device\":\"sensor\",\"calibration\":1.234}";
    cJSON *parsed = cJSON_Parse(json_blob);
    cJSON_AddItemToObject(root, "import", parsed);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "import");
    cJSON_AddItemToObject(settings, "imported_config", detached);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}