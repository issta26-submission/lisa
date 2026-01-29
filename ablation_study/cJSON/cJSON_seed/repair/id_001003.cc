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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON *name_item = cJSON_AddStringToObject(config, "name", "original");
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON_SetValuestring(name_item, "modified");

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(config, "name");
    const char *got_name_str = cJSON_GetStringValue(got_name);
    cJSON_AddStringToObject(root, "name_copy", got_name_str);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}