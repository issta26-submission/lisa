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
//<ID> 1420
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
    cJSON_AddTrueToObject(config, "enabled");
    cJSON_AddNumberToObject(config, "threshold", 3.14159);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", config_copy);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "item_count", (double)count);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_enabled = cJSON_GetObjectItem(got_config, "enabled");
    cJSON_bool enabled = cJSON_IsTrue(got_enabled);
    cJSON *thr_item = cJSON_GetObjectItem(got_config, "threshold");
    double threshold = cJSON_GetNumberValue(thr_item);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, (size_t)(len + 16));
    scratch[0] = len ? unfmt[0] : 0;
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + ((int)enabled % 10));
    scratch[3] = (char)('0' + (((int)threshold) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}