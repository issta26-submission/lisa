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
//<ID> 1302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON *ver_item = cJSON_AddStringToObject(metadata, "version", "1.2.3");
    cJSON *author_item = cJSON_AddStringToObject(metadata, "author", "Alice");
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(root, "enabled", enabled);
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "initial");

    // step 2: Configure
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled);
    const char *version_str = cJSON_GetStringValue(ver_item);
    const char *author_str = cJSON_GetStringValue(author_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = version_str ? version_str[0] : 0;
    ((char *)scratch)[2] = author_str ? author_str[0] : 0;
    ((char *)scratch)[3] = (char)(enabled_is_bool + enabled_is_true);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}