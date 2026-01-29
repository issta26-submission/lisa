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
//<ID> 745
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
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "stats", stats);
    cJSON *count = cJSON_AddNumberToObject(stats, "count", 42.0);
    cJSON *temp = cJSON_AddNumberToObject(stats, "temp", 3.14);

    // step 2: Configure
    cJSON *detached_count = cJSON_DetachItemFromObject(stats, "count");
    double detached_value = cJSON_GetNumberValue(detached_count);
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    cJSON *added_number = cJSON_AddNumberToObject(root, "extracted_value", detached_value * (double)meta_is_object);
    cJSON_AddItemToObject(root, "reattached_count", detached_count);

    // step 3: Operate and Validate
    double verify_added = cJSON_GetNumberValue(added_number);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}