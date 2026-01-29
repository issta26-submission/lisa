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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_flag = NULL;
    cJSON *new_flag = NULL;
    cJSON *detached_item = NULL;
    char *printed_root = NULL;
    char *printed_detached = NULL;
    cJSON_bool replace_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", old_flag);

    // step 3: Operate / Validate
    new_flag = cJSON_CreateBool(1);
    replace_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_flag);
    printed_root = cJSON_PrintUnformatted(root);
    detached_item = cJSON_DetachItemFromObject(root, "flag");
    printed_detached = cJSON_PrintUnformatted(detached_item);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}