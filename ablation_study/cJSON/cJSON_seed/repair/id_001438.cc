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
//<ID> 1438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddFalseToObject(config, "enabled");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(13.0));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "sample");
    cJSON_AddItemToObject(root, "meta", meta);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *enabled = cJSON_GetObjectItem(cfg, "enabled");
    cJSON_bool enabled_is_array = cJSON_IsArray(enabled);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *buf = (char *)cJSON_malloc(len + 16);
    memset(buf, 0, len + 16);
    buf[0] = len > 0 ? unfmt[0] : ' ';
    buf[1] = (char)('0' + (count % 10));
    buf[2] = (char)('0' + ((count / 10) % 10));
    buf[3] = enabled_is_array ? 'A' : 'N';
    buf[4] = items_is_array ? 'A' : 'N';

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}