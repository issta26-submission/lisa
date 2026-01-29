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
//<ID> 1429
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
    cJSON_AddNumberToObject(config, "version", 2.5);
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    cJSON_AddStringToObject(root, "note", "created");

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *enabled = cJSON_GetObjectItem(cfg, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(enabled);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *buf = (char *)cJSON_malloc(len + 16);
    memset(buf, 0, len + 16);
    buf[0] = len > 0 ? unfmt[0] : ' ';
    buf[1] = (char)('0' + (count % 10));
    buf[2] = (char)(is_true ? '1' : '0');

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}