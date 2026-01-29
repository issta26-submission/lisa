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
//<ID> 1492
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
    cJSON *name = cJSON_CreateString("alice");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *status = cJSON_CreateString("init");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "active", 1);
    char *set_ret = cJSON_SetValuestring(status, "running");

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(name);
    char *status_str = cJSON_GetStringValue(status);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = status_str ? status_str[0] : 'y';
    scratch[2] = (char)('0' + (int)cJSON_IsTrue(active_item));
    scratch[3] = set_ret ? set_ret[0] : 'z';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}