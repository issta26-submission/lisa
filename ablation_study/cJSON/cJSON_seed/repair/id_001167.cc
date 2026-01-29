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
//<ID> 1167
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
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *count_item = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(payload, "count", count_item);
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON *nullable_item = cJSON_CreateNull();
    cJSON_AddItemToObject(payload, "nullable", nullable_item);

    // step 2: Configure
    cJSON_AddTrueToObject(meta, "active");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = out ? out[1] : '\0';

    // step 3: Operate and Validate
    cJSON *payload_count_ref = cJSON_GetObjectItem(payload, "count");
    double count_val = cJSON_GetNumberValue(payload_count_ref);
    double obj_flag = (double)cJSON_IsObject(payload);
    double updated = cJSON_SetNumberHelper(payload_count_ref, count_val + obj_flag);
    cJSON *detached = cJSON_DetachItemFromObject(payload, "nullable");
    double was_null = (double)cJSON_IsNull(detached);
    cJSON_Delete(detached);
    cJSON *computed = cJSON_CreateNumber(updated + was_null);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}