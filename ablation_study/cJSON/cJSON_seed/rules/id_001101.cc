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
//<ID> 1101
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
    cJSON *status = NULL;
    cJSON *new_status_bool = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    status = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "status", status);
    new_status_bool = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "status", new_status_bool);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "name");
    cJSON_AddItemToObject(root, "alias", detached);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}