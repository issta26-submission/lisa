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
//<ID> 882
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(cfg, "answer", num_item);
    cJSON *flag_item = cJSON_AddTrueToObject(cfg, "enabled");

    // step 3: Operate
    const int buf_size = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_size);
    memset(buffer, 0, (size_t)buf_size);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, buf_size, 0);
    char *out_unformatted = cJSON_PrintUnformatted(root);
    (void)pre_ok;
    (void)out_unformatted;

    // step 4: Validate & Cleanup
    cJSON_bool flag_is_true = cJSON_IsTrue(flag_item);
    (void)flag_is_true;
    cJSON_free(out_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}