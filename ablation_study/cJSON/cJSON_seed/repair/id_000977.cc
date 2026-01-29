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
//<ID> 977
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *params = cJSON_CreateObject();
    cJSON *val_num = cJSON_CreateNumber(42.0);
    cJSON *val_str = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(params, "num", val_num);
    cJSON_AddItemToObjectCS(params, "str", val_str);
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON *info = cJSON_CreateString("configuration");
    cJSON_AddItemToObjectCS(cfg, "info", info);
    cJSON_AddItemToObjectCS(root, "params", params);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "params");
    cJSON_AddItemToObjectCS(cfg, "migrated_params", detached);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *migrated = cJSON_GetObjectItem(cfg, "migrated_params");
    cJSON *numItem = cJSON_GetObjectItem(migrated, "num");
    double numValue = cJSON_GetNumberValue(numItem);
    (void)numValue;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}