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
//<ID> 320
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

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "threshold", threshold);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *version = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(meta, "version", version);

    // step 3: Operate and Validate
    cJSON *got_threshold = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(got_threshold);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_version = cJSON_GetObjectItem(got_meta, "version");
    double ver_val = cJSON_GetNumberValue(got_version);
    cJSON *sum = cJSON_CreateNumber(thr_val + ver_val);
    cJSON_AddItemToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}