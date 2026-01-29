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
//<ID> 1203
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
    cJSON *name = cJSON_CreateString("example");
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON *threshold = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "threshold", threshold);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "maybe", maybe);
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(config, "status", status);

    // step 3: Operate and Validate
    cJSON *maybe_item = cJSON_GetObjectItem(config, "maybe");
    cJSON_bool was_null = cJSON_IsNull(maybe_item);
    cJSON *was_null_flag = cJSON_CreateBool(was_null);
    cJSON_AddItemToObject(config, "was_null", was_null_flag);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = out ? out[1] : '\0';

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}