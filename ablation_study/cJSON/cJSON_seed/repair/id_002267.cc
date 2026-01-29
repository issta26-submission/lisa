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
//<ID> 2267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    const char *version_str = cJSON_Version();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(meta, "version", ver_item);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "value", cJSON_CreateNumber(42.0));
    cJSON_AddItemToObject(config, "child", child);
    cJSON *backup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "backup", backup);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement, "value", cJSON_CreateNumber(99.0));
    cJSON_ReplaceItemViaPointer(root, backup, replacement);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    char *got_ver = cJSON_GetStringValue(cJSON_GetObjectItem(got_meta, "version"));
    buffer[1] = got_ver ? got_ver[0] : '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}