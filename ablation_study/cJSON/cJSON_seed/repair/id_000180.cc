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
//<ID> 180
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
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "answer", answer);
    cJSON *offset = cJSON_CreateNumber(8.0);
    cJSON_AddItemToObject(config, "offset", offset);
    cJSON *raw_added = cJSON_AddRawToObject(config, "raw_blob", "{\"nested\":true,\"value\":123}");

    // step 2: Configure
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *answer_ref = cJSON_GetObjectItem(config_ref, "answer");
    cJSON *offset_ref = cJSON_GetObjectItem(config_ref, "offset");
    double sum_val = cJSON_GetNumberValue(answer_ref) + cJSON_GetNumberValue(offset_ref);
    cJSON *sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate and Validate
    cJSON *raw_ref = cJSON_GetObjectItem(config_ref, "raw_blob");
    cJSON *detached_raw = cJSON_DetachItemViaPointer(config_ref, raw_ref);
    cJSON_AddItemToObject(root, "detached_raw", detached_raw);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}