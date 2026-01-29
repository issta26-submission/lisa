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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version_str = cJSON_Version();
    const char name_literal[] = "example_name";
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString(name_literal);
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON *enabled_item = cJSON_CreateTrue();
    cJSON *retrieved_name = NULL;
    cJSON *retrieved_enabled = NULL;
    const char *retrieved_name_str = NULL;
    cJSON_bool enabled_is_true = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "version", version_item);
    cJSON_AddItemToObject(root, "enabled", enabled_item);

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    retrieved_name_str = cJSON_GetStringValue(retrieved_name);
    enabled_is_true = cJSON_IsTrue(retrieved_enabled);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)retrieved_name_str;
    (void)enabled_is_true;
    return 66;
}