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
//<ID> 889
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
    cJSON *enabled_item = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *answer_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(cfg, "answer", answer_item);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;

    // step 3: Operate
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 256, 0);
    (void)pre_ok;
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_bool enabled_truth = cJSON_IsTrue(enabled_item);
    int flag = (int)enabled_truth;
    double answer_val = cJSON_GetNumberValue(answer_item);
    double adjusted_val = answer_val + flag;
    cJSON_AddNumberToObject(cfg, "adjusted", adjusted_val);

    // step 4: Validate & Cleanup
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}