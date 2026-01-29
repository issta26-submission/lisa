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
//<ID> 184
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
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(config, "count", count);
    cJSON *meta = cJSON_AddRawToObject(config, "metadata", "{\"id\":1,\"flag\":true}");

    // step 2: Configure
    cJSON *threshold_item = cJSON_GetObjectItem(config, "threshold");
    double tval = cJSON_GetNumberValue(threshold_item);
    cJSON *scaled = cJSON_CreateNumber(tval * 2.0);
    cJSON_AddItemToObject(config, "scaled", scaled);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemViaPointer(config, meta);
    cJSON_AddItemToObject(root, "detached_metadata", detached_meta);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON *sum = cJSON_CreateNumber(tval + cJSON_GetNumberValue(count));
    cJSON_AddItemToObject(summary, "sum", sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}