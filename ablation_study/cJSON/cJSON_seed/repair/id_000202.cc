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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(payload, "flags");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, false_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(flags, null_item);
    cJSON *dup_false = cJSON_Duplicate(false_item, 0);
    cJSON_AddItemToObject(payload, "dup_false", dup_false);

    // step 3: Operate and Validate
    cJSON *retrieved_payload = cJSON_GetObjectItem(root, "payload");
    cJSON *retrieved_flags = cJSON_GetObjectItem(retrieved_payload, "flags");
    int flag_count = cJSON_GetArraySize(retrieved_flags);
    cJSON_AddNumberToObject(payload, "flag_count", (double)flag_count);
    cJSON *retrieved_dup = cJSON_GetObjectItem(retrieved_payload, "dup_false");
    (void)retrieved_dup; // used to satisfy meaningful flow

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}