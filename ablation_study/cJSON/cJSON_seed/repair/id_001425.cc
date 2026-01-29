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
//<ID> 1425
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
    cJSON_AddNumberToObject(config, "version", 2.0);
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *fetched_config = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_item = cJSON_GetObjectItem(fetched_config, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *scratch = (char *)cJSON_malloc((size_t)buf_len);
    memset(scratch, 0, (size_t)buf_len);
    scratch[0] = json[0];
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + (int)is_enabled);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}