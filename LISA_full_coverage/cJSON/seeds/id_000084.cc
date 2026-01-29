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
//<ID> 84
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, str);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(config, "enabled");
    cJSON *ver = cJSON_CreateString("v1.0");
    cJSON_AddItemToObject(config, "version", ver);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, config);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_bool detached_is_array = cJSON_IsArray(detached);
    char *printed = cJSON_Print(root);
    int summary = (int)items_is_array + (int)detached_is_array + (int)printed[0] + (int)(flag != NULL);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}