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
//<ID> 1060
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
    const char *ver_const = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(ver_const);
    cJSON_AddItemToObjectCS(meta, "version", version_item);

    // step 2: Configure
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "info", info);
    cJSON *name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObjectCS(info, "name", name_item);
    const char *name_val = cJSON_GetStringValue(name_item);

    // step 3: Operate
    cJSON *nested = cJSON_AddObjectToObject(info, "nested");
    cJSON_AddItemToObjectCS(nested, "derived_version", cJSON_CreateString(name_val));
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    const char *version_val = cJSON_GetStringValue(version_item);
    cJSON_AddItemToObjectCS(root, "version_copy", cJSON_CreateString(version_val));
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}