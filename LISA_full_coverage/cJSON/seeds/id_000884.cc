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
//<ID> 884
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
    cJSON *enabled = cJSON_AddTrueToObject(cfg, "enabled");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(cfg, "answer", num);

    // step 2: Configure
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);

    // step 3: Operate
    cJSON_bool is_enabled = cJSON_IsTrue(enabled);
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)pre_ok;
    (void)is_enabled;
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}