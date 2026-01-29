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
//<ID> 1205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON *active = cJSON_CreateBool(1);
    cJSON *score = cJSON_CreateNumber(42.5);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddItemToObject(root, "score", score);
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *tag = cJSON_CreateString("v1");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate and Validate
    cJSON *maybe_ptr = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_null = cJSON_IsNull(maybe_ptr);
    cJSON *score_ptr = cJSON_GetObjectItem(root, "score");
    double score_val = cJSON_GetNumberValue(score_ptr);
    double final_val = score_val + (double)is_null;
    cJSON *final_item = cJSON_CreateNumber(final_val);
    cJSON_AddItemToObject(root, "final", final_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}