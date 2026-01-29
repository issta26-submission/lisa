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
//<ID> 1353
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
    cJSON *initial_threshold = cJSON_AddNumberToObject(config, "threshold", 3.14);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *version_item = cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate and Validate
    cJSON *replacement_threshold = cJSON_CreateNumber(9.81);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(config, "threshold", replacement_threshold);
    cJSON *current_threshold_item = cJSON_GetObjectItem(config, "threshold");
    cJSON *current_version_item = cJSON_GetObjectItem(meta, "version");
    double val_threshold = cJSON_GetNumberValue(current_threshold_item);
    double val_version = cJSON_GetNumberValue(current_version_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (replaced ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (((int)val_threshold) % 10));
    ((char *)scratch)[3] = (char)('0' + (((int)val_version) % 10));
    ((char *)scratch)[4] = (char)('0' + ((current_threshold_item && current_version_item) ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}