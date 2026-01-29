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
//<ID> 880
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
    char *buf = (char *)cJSON_malloc(32);
    memset(buf, 0, 32);
    memcpy(buf, "{\"k\":1}", 8);

    // step 2: Configure
    cJSON *parsed = cJSON_Parse(buf);
    cJSON_AddItemToObject(cfg, "embedded", parsed);
    cJSON *true_item = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON_bool enabled = cJSON_IsTrue(true_item);
    (void)enabled;

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(buf);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}