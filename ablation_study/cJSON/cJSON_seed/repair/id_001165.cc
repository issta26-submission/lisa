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
//<ID> 1165
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
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(root, "note", "initial");

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON_AddNullToObject(root, "obsolete");
    cJSON *obsolete = cJSON_GetObjectItem(root, "obsolete");
    cJSON_bool was_null = cJSON_IsNull(obsolete);
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    cJSON_bool was_object = cJSON_IsObject(detached);
    cJSON *detached_threshold = cJSON_GetObjectItem(detached, "threshold");
    double updated = cJSON_SetNumberHelper(detached_threshold, 42.5);
    cJSON_AddNumberToObject(root, "computed", updated);
    double flag_sum = (double)was_object + (double)was_null;
    cJSON_AddNumberToObject(root, "flag_sum", flag_sum);
    cJSON_AddItemToObject(root, "restored", detached);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}