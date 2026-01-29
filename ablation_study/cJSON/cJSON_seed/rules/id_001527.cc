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
//<ID> 1527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *items = NULL;
    cJSON *meta = NULL;
    cJSON *meta_version = NULL;
    cJSON *retrieved = NULL;
    const char *lib_version = NULL;
    const char *meta_version_str = NULL;
    int items_count = 0;
    char json[] = "{ \"items\": [10, 20, 30], \"meta\": { \"version\": \"1.2.3\" } }";
    size_t len = sizeof(json) - 1;
    char prebuf[64];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, len);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    items = cJSON_GetObjectItem(parsed, "items");
    meta = cJSON_GetObjectItem(parsed, "meta");

    // step 3: Operate / Validate
    items_count = cJSON_GetArraySize(items);
    meta_version = cJSON_GetObjectItem(meta, "version");
    meta_version_str = cJSON_GetStringValue(meta_version);
    lib_version = cJSON_Version();
    retrieved = cJSON_GetObjectItem(root, "imported");
    (void)prebuf;
    (void)items_count;
    (void)meta_version_str;
    (void)lib_version;
    (void)retrieved;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}