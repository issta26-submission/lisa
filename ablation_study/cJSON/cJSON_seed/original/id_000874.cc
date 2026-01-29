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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(meta, "optional", optional_null);

    // step 2: Configure
    const char *version_literal = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_literal);
    cJSON_AddItemToObjectCS(root, "version", version_item);
    const char *version_value = cJSON_GetStringValue(version_item);
    cJSON *version_copy = cJSON_CreateString(version_value);
    cJSON_AddItemToObjectCS(meta, "version_copy", version_copy);

    // step 3: Operate
    char *unformatted = cJSON_PrintUnformatted(root);
    char *formatted = cJSON_Print(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    (void)pre_ok;

    // step 4: Validate & Cleanup
    const char *verify = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "version_copy"));
    (void)verify;
    cJSON_free(unformatted);
    cJSON_free(formatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}