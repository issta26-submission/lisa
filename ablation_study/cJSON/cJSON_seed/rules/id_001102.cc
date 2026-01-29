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
//<ID> 1102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *new_name = NULL;
    cJSON *active = NULL;
    cJSON *detached_active = NULL;
    cJSON *got = NULL;
    char *printed1 = NULL;
    char *printed2 = NULL;
    char *printed_detached = NULL;
    char *got_str = NULL;
    cJSON_bool replaced_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name);
    active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active);

    // step 3: Operate / Validate
    printed1 = cJSON_PrintUnformatted(root);
    new_name = cJSON_CreateString("replaced_name");
    replaced_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "name", new_name);
    detached_active = cJSON_DetachItemFromObject(root, "active");
    printed2 = cJSON_PrintUnformatted(root);
    printed_detached = cJSON_PrintUnformatted(detached_active);
    got = cJSON_GetObjectItem(root, "name");
    got_str = cJSON_GetStringValue(got);

    // step 4: Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(printed_detached);
    cJSON_Delete(detached_active);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}