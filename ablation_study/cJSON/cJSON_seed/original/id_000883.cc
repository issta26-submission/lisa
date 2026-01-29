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
//<ID> 883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(cfg, "threshold", threshold);

    // step 3: Operate
    cJSON_bool is_enabled = cJSON_IsTrue(enabled);
    void *temp_buf = cJSON_malloc(128);
    memset(temp_buf, 0, 128);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)is_enabled;
    cJSON_free(temp_buf);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}