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
//<ID> 330
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_str = cJSON_CreateString("example_name");
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON *version_str = cJSON_CreateString(version);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *retrieved_meta = NULL;
    cJSON *retrieved_name = NULL;
    cJSON *retrieved_enabled = NULL;
    const char *retrieved_name_val = NULL;
    cJSON_bool is_enabled = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", name_str);
    cJSON_AddItemToObject(root, "enabled", enabled_true);
    cJSON_AddItemToObject(root, "version", version_str);

    // step 3: Operate / Validate
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    retrieved_name = cJSON_GetObjectItem(retrieved_meta, "name");
    retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    retrieved_name_val = cJSON_GetStringValue(retrieved_name);
    is_enabled = cJSON_IsTrue(retrieved_enabled);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = retrieved_name_val[0];
    buffer[1] = (char)('0' + (int)is_enabled);
    buffer[2] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}