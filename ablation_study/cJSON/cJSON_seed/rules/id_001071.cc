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
//<ID> 1071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *original_false = NULL;
    cJSON *replacement_false = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    cJSON_bool is_bool_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    original_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", original_false);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_bool_result = cJSON_IsBool(got_item);
    cJSON_AddBoolToObject(root, "was_bool", is_bool_result);
    replacement_false = cJSON_CreateFalse();
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement_false);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}