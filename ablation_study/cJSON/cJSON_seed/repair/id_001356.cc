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
//<ID> 1356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *threshold_item = cJSON_AddNumberToObject(settings, "threshold", 3.14);
    cJSON *limit_item = cJSON_AddNumberToObject(settings, "limit", 100.0);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced_ok = cJSON_ReplaceItemInObject(settings, "threshold", replacement);

    // step 3: Operate and Validate
    double val_threshold = cJSON_GetNumberValue(threshold_item);
    double val_limit = cJSON_GetNumberValue(limit_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + ((int)val_threshold % 10));
    ((char *)scratch)[2] = (char)('0' + ((int)val_limit % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced_ok ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}