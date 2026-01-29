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
//<ID> 1079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *initial_flag = NULL;
    cJSON *replacement_flag = NULL;
    cJSON *got_flag = NULL;
    char *printed = NULL;
    cJSON_bool replace_res = 0;
    cJSON_bool is_bool = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    initial_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", initial_flag);

    // step 3: Operate / Validate
    replacement_flag = cJSON_CreateFalse();
    replace_res = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement_flag);
    got_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    is_bool = cJSON_IsBool(got_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}