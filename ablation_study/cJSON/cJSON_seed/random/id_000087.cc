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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *flag_repl = (cJSON *)0;
    cJSON *snapshot = (cJSON *)0;

    // step 2: Setup - assemble structure
    cJSON_AddItemToObject(settings, "enabled", flag_false);
    cJSON_AddItemToObject(root, "settings", settings);

    // step 3: Operate - replace the 'enabled' item via pointer with a true value
    flag_repl = cJSON_CreateTrue();
    cJSON_ReplaceItemViaPointer(settings, flag_false, flag_repl);

    // step 4: Validate - create a duplicate snapshot of settings and attach
    snapshot = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 5: Cleanup - delete root which frees children including replacement and snapshot
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}