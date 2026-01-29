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
//<ID> 1967
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *ver = cJSON_CreateString("1.2.3");
    cJSON_AddItemToObject(meta, "version", ver);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", enabled);

    // step 2: Configure
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *mode = cJSON_CreateString("standard");
    cJSON_AddItemToObject(config, "mode", mode);
    cJSON *owner = cJSON_CreateString("tester");
    cJSON_AddItemToObject(config, "owner", owner);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItem(got_meta, "version");
    cJSON *got_enabled = cJSON_GetObjectItem(got_meta, "enabled");
    const char *version_str = cJSON_GetStringValue(got_version);
    cJSON_bool enabled_is_false = cJSON_IsFalse(got_enabled);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + ((version_str != ((const char *)0)) ? (version_str[0] % 10) : 0));
    buffer[1] = (char)('0' + (enabled_is_false % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}