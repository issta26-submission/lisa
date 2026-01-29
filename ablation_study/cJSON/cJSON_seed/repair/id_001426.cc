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
//<ID> 1426
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
    cJSON_AddTrueToObject(config, "enabled");
    cJSON_AddNumberToObject(config, "ratio", 3.14159);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "cJSON_test");

    // step 3: Operate and Validate
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    cJSON *items_obj = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(items_obj);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : 0;
    char *buf = (char *)cJSON_malloc(len + 8);
    memset(buf, 0, (size_t)(len + 8));
    buf[0] = len ? unfmt[0] : '\0';
    buf[1] = (char)('0' + (int)(is_enabled != 0));
    buf[2] = (char)('0' + (count % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}