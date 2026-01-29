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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *info = cJSON_CreateString("initial_info");
    cJSON_AddItemToObjectCS(root, "info", info);

    // step 2: Configure
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObjectCS(meta, "version", version_item);
    const char *info_value = cJSON_GetStringValue(info);
    cJSON *info_copy = cJSON_CreateString(info_value);
    cJSON_AddItemToObjectCS(meta, "info_copy", info_copy);

    // step 3: Operate
    char *pretty = cJSON_Print(root);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}