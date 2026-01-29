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
//<ID> 795
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
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", list);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "version", 2.0);
    cJSON_AddStringToObject(config, "name", "example");
    cJSON_AddTrueToObject(config, "enabled");
    cJSON_AddItemToArray(list, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(list, 1);
    cJSON_bool second_is_invalid = cJSON_IsInvalid(second);
    (void)second_is_invalid;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}