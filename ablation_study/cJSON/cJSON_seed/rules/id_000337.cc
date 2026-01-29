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
//<ID> 337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *version_node = cJSON_CreateString(version);
    cJSON *enabled_node = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *retrieved_version = NULL;
    cJSON *retrieved_enabled = NULL;
    cJSON_bool enabled_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "version", version_node);
    cJSON_AddItemToObject(root, "enabled", enabled_node);
    cJSON_AddNumberToObject(root, "answer", 66.0);

    // step 3: Operate / Validate
    retrieved_version = cJSON_GetObjectItem(root, "version");
    retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    enabled_true = cJSON_IsTrue(retrieved_enabled);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)enabled_true);
    buffer[1] = retrieved_version->valuestring[0];
    buffer[2] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}