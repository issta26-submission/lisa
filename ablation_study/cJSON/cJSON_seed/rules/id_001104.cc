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
//<ID> 1104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_name = NULL;
    cJSON *flag = NULL;
    cJSON *replacement = NULL;
    cJSON *detached_flag = NULL;
    cJSON *got_name = NULL;
    char *printed_root = NULL;
    char *printed_detached = NULL;
    const char *name_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_name = cJSON_CreateString("original_name");
    cJSON_AddItemToObject(root, "name", old_name);
    flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced_name");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement);
    detached_flag = cJSON_DetachItemFromObject(root, "active");
    got_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_str = cJSON_GetStringValue(got_name);
    printed_root = cJSON_PrintUnformatted(root);
    printed_detached = cJSON_PrintUnformatted(detached_flag);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached_flag);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}