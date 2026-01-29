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
//<ID> 1282
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
    cJSON_AddItemToObject(root, "config", config);
    cJSON *ver = cJSON_CreateString("1.0");
    cJSON_AddItemToObject(config, "version", ver);
    cJSON *threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 2: Configure
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *old_threshold = cJSON_GetObjectItem(got_config, "threshold");
    cJSON *new_threshold = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemViaPointer(got_config, old_threshold, new_threshold);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}