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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *enabled_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "enabled", enabled_item);
    cJSON_AddRawToObject(root, "raw", "{\"inner\":42, \"flag\":true}");

    // step 3: Operate / Validate
    cJSON *got_enabled = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool is_enabled_true = cJSON_IsTrue(got_enabled);
    cJSON *count_item = cJSON_CreateNumber((double)is_enabled_true);
    cJSON_AddItemToObject(root, "count", count_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}