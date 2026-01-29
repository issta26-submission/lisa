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
//<ID> 1436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddFalseToObject(config, "enabled");

    // step 2: Configure
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *enabled = cJSON_GetObjectItem(cfg, "enabled");
    cJSON_bool items_is_array = cJSON_IsArray(items);
    double second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));

    // step 3: Operate and Validate
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = strlen(unfmt);
    char *scratch = (char *)cJSON_malloc(len + 8);
    memset(scratch, 0, len + 8);
    scratch[0] = unfmt[0];
    scratch[1] = (char)('0' + (int)items_is_array);
    scratch[2] = (char)('0' + ((int)second_value % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}