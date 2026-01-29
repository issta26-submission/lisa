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
//<ID> 878
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *name = cJSON_CreateString("device123");
    cJSON_AddItemToObjectCS(meta, "name", name);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(meta, "missing", missing);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToObjectCS(meta, "lib_version", ver_item);

    // step 2: Configure
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    char *out = cJSON_PrintUnformatted(root);
    char *name_val = cJSON_GetStringValue(name);
    cJSON *name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObjectCS(root, "name_ref", name_ref);

    // step 3: Operate
    cJSON *snapshot = cJSON_Duplicate(root, 1);
    char *snap_str = cJSON_PrintUnformatted(snapshot);
    cJSON_AddItemToObjectCS(root, "snapshot", snapshot);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(snap_str);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}