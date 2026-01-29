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
//<ID> 784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *source = cJSON_Parse("{\"settings\":{\"mode\":\"auto\",\"threshold\":2.5},\"version\":1.0}");

    // step 2: Configure
    cJSON_AddStringToObject(root, "session", "alpha");
    cJSON_AddNumberToObject(root, "timestamp", 1618033988.0);

    // step 3: Operate and Validate
    cJSON *settings = cJSON_DetachItemFromObject(source, "settings");
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *version_item = cJSON_GetObjectItem(source, "version");
    double version_value = cJSON_GetNumberValue(version_item);
    cJSON_AddNumberToObject(root, "source_version", version_value);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(source);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}