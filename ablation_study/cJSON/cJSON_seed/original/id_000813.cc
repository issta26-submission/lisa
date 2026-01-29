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
//<ID> 813
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
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON *name_item = cJSON_AddStringToObject(config, "name", "example");

    // step 2: Configure
    double new_value = cJSON_SetNumberHelper(num_item, 42.0);
    cJSON_AddItemToObject(config, "value", num_item);
    cJSON_AddStringToObject(root, "root_note", "initialized");

    // step 3: Operate
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(compact);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);
    cJSON *retrieved_config = cJSON_GetObjectItem(root, "config");
    double retrieved_value = cJSON_GetNumberValue(cJSON_GetObjectItem(retrieved_config, "value"));
    const char *retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_config, "name"));
    (void)new_value;
    (void)retrieved_value;
    (void)retrieved_name;
    (void)name_item;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}