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
//<ID> 1007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *null_item = NULL;
    cJSON *retrieved_name_item = NULL;
    char *unformatted = NULL;
    const char *device_name = "device_123";
    const char *extracted_name = NULL;
    cJSON_bool optional_is_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(device_name);
    cJSON_AddItemToObject(root, "name", name_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_item);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted(root);
    retrieved_name_item = cJSON_GetObjectItem(root, "name");
    extracted_name = cJSON_GetStringValue(retrieved_name_item);
    optional_is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}