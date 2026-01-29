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
//<ID> 720
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(list, false_item);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_SetValuestring(str_item, "updated");
    cJSON_AddItemToArray(list, str_item);
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(list, num_item);

    // step 3: Operate and Validate
    cJSON *config_ref = cJSON_GetObjectItemCaseSensitive(root, "config");
    cJSON *list_ref = cJSON_GetObjectItemCaseSensitive(root, "list");
    int list_size = cJSON_GetArraySize(list_ref);
    cJSON_AddNumberToObject(config_ref, "list_size", (double)list_size);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}