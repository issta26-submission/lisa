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
//<ID> 1256
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "items", items);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddStringToObject(config, "name", "example");
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddStringToObject(sub, "mode", "fast");
    cJSON_AddItemToObject(config, "sub", sub);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_sub = cJSON_GetObjectItem(got_config, "sub");
    cJSON_bool is_invalid = cJSON_IsInvalid(got_sub);
    cJSON *detached = cJSON_DetachItemViaPointer(got_config, got_sub);
    cJSON_AddItemToObject(root, "detached_sub", detached);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    cJSON *got_detached = cJSON_GetObjectItem(root, "detached_sub");
    cJSON *mode_item = cJSON_GetObjectItem(got_detached, "mode");
    char *mode_str = cJSON_GetStringValue(mode_item);
    (void)mode_str;
    (void)is_invalid;
    (void)printed_ok;
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}