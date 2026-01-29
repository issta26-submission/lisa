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
//<ID> 1004
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
    cJSON *enabled_item = NULL;
    cJSON *name_lookup = NULL;
    char *printed = NULL;
    char *name_value = NULL;
    cJSON_bool optional_is_null = 0;
    cJSON_bool enabled_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("sensor42");
    cJSON_AddItemToObject(root, "name", name_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_item);
    enabled_item = cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate / Validate
    name_lookup = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(name_lookup);
    optional_is_null = cJSON_IsNull(cJSON_GetObjectItem(root, "optional"));
    enabled_is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}