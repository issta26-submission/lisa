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
//<ID> 1077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *initial_bool = NULL;
    cJSON *replacement_false = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool is_bool = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    initial_bool = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", initial_bool);

    // step 3: Operate / Validate
    replacement_false = cJSON_CreateFalse();
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "enabled", replacement_false);
    got = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    is_bool = cJSON_IsBool(got);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}