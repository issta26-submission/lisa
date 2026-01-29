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
//<ID> 1206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(info, "enabled", enabled);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(info, "count", count);

    // step 2: Configure
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe);
    cJSON *is_null_flag = cJSON_CreateBool(maybe_is_null);
    cJSON_AddItemToObject(root, "maybe_is_null", is_null_flag);
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    double cnt_val = cJSON_GetNumberValue(count);
    cJSON *next_count = cJSON_CreateNumber(cnt_val + 1.0);
    cJSON_AddItemToObject(info, "next_count", next_count);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}