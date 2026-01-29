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
//<ID> 811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "threshold", 3.14159);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "version", 2.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    int values_count = cJSON_GetArraySize(values);
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(config, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);

    // step 3: Operate and Validate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_AddItemToObject(config, "meta_detached", detached_meta);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_meta = cJSON_PrintUnformatted(detached_meta ? detached_meta : config);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}