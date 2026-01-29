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
//<ID> 1182
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
    cJSON *mode_old = NULL;
    cJSON *mode_new = NULL;
    cJSON *got_config = NULL;
    cJSON *got_mode = NULL;
    cJSON *orphan = NULL;
    char *printed = NULL;
    double mode_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    mode_old = cJSON_CreateString("legacy");
    cJSON_AddItemToObject(config, "mode", mode_old);
    cJSON_AddItemToObject(config, "threshold", cJSON_CreateNumber(3.14));
    cJSON_AddTrueToObject(root, "enabled");
    orphan = cJSON_CreateString("temporary_orphan");

    // step 3: Operate / Validate
    got_config = cJSON_GetObjectItem(root, "config");
    mode_new = cJSON_CreateNumber(2.0);
    cJSON_ReplaceItemViaPointer(config, mode_old, mode_new);
    got_mode = cJSON_GetObjectItem(got_config, "mode");
    mode_value = cJSON_GetNumberValue(got_mode);
    printed = cJSON_PrintUnformatted(root);
    (void)mode_value;
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(orphan);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}