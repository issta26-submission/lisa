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
//<ID> 1207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *score = cJSON_CreateNumber(12.5);
    cJSON_AddItemToObject(root, "score", score);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *version = cJSON_CreateString("v1.0");
    cJSON_AddItemToObject(meta, "version", version);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    (void)name_val;
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON *score_item = cJSON_GetObjectItem(root, "score");
    double score_val = cJSON_GetNumberValue(score_item);
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe_item);
    double computed = score_val + (double)enabled_flag * 10.0 + (double)(!maybe_is_null) * 5.0;
    cJSON *final_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "final", final_num);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}