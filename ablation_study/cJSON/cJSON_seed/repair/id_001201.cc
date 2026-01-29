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
//<ID> 1201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *header = cJSON_CreateObject();
    cJSON *body = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("alpha");
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON *maybe = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "header", header);
    cJSON_AddItemToObject(root, "body", body);
    cJSON_AddItemToObject(header, "name", name);
    cJSON_AddItemToObject(header, "enabled", enabled);
    cJSON_AddItemToObject(header, "count", count);
    cJSON_AddItemToObject(body, "maybe", maybe);

    // step 3: Operate and Validate
    cJSON *retrieved_count = cJSON_GetObjectItem(header, "count");
    double count_val = cJSON_GetNumberValue(retrieved_count);
    cJSON *retrieved_maybe = cJSON_GetObjectItem(body, "maybe");
    cJSON_bool is_null = cJSON_IsNull(retrieved_maybe);
    double final_val = count_val + (is_null ? 1.0 : 0.0);
    cJSON *final_num = cJSON_CreateNumber(final_val);
    cJSON_AddItemToObject(root, "final", final_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}