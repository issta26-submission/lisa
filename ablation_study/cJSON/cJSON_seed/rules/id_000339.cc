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
//<ID> 339
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
    cJSON *config = cJSON_CreateObject();
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON *name_str = cJSON_CreateString("fuzz_test");
    cJSON *retrieved_name = NULL;
    cJSON *retrieved_config = NULL;
    cJSON *retrieved_enabled = NULL;
    cJSON_bool is_enabled = 0;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "enabled", enabled_true);
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_config = cJSON_GetObjectItem(root, "config");
    retrieved_enabled = cJSON_GetObjectItem(retrieved_config, "enabled");
    is_enabled = cJSON_IsTrue(retrieved_enabled);
    printed = cJSON_PrintUnformatted(root);
    const char *name_val = cJSON_GetStringValue(retrieved_name);
    buffer[0] = (char)('0' + (int)is_enabled);
    buffer[1] = name_val[0];
    buffer[2] = version[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}