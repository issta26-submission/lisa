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
//<ID> 1253
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
    cJSON_AddStringToObject(config, "mode", "test");
    cJSON_AddNumberToObject(config, "value", 123.0);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, n2);
    cJSON *config_ref = cJSON_CreateObjectReference(config);
    cJSON_AddItemToObject(root, "config_ref", config_ref);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *mode_item = cJSON_GetObjectItem(got_config, "mode");
    cJSON_bool mode_invalid = cJSON_IsInvalid(mode_item);
    (void)mode_invalid;
    cJSON *got_ref = cJSON_GetObjectItem(root, "config_ref");
    cJSON *detached = cJSON_DetachItemViaPointer(root, got_ref);
    cJSON_bool detached_invalid = cJSON_IsInvalid(detached);
    (void)detached_invalid;
    int buf_len = 2048;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 0);
    (void)buf[0];

    // step 4: Validate & Cleanup
    cJSON_Delete(detached);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}