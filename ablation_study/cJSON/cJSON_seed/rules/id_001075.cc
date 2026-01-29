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
//<ID> 1075
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *config = NULL;
    cJSON *got_config = NULL;
    cJSON *enabled_item = NULL;
    cJSON *replacement_true = NULL;
    char *printed = NULL;
    cJSON_bool replace_result = 0;
    cJSON_bool is_bool = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObject(config, "enabled", cJSON_CreateFalse());

    // step 3: Operate / Validate
    got_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    replacement_true = cJSON_CreateTrue();
    replace_result = cJSON_ReplaceItemInObjectCaseSensitive(got_config, "enabled", replacement_true);
    enabled_item = cJSON_GetObjectItemCaseSensitive(got_config, "enabled");
    is_bool = cJSON_IsBool(enabled_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}