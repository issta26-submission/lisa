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
//<ID> 1073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *device = NULL;
    cJSON *old_status = NULL;
    cJSON *new_status_false = NULL;
    cJSON *got_status = NULL;
    char *printed = NULL;
    cJSON_bool got_is_bool = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    device = cJSON_CreateObject();
    cJSON_AddStringToObject(device, "status", "active");
    cJSON_AddNumberToObject(device, "count", 3);
    cJSON_AddItemToObject(root, "device", device);

    // step 3: Operate / Validate
    old_status = cJSON_GetObjectItemCaseSensitive(device, "status");
    new_status_false = cJSON_CreateFalse();
    cJSON_ReplaceItemInObjectCaseSensitive(device, "status", new_status_false);
    got_status = cJSON_GetObjectItemCaseSensitive(device, "status");
    got_is_bool = cJSON_IsBool(got_status);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}