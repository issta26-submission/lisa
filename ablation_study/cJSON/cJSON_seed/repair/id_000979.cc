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
//<ID> 979
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNumberToObject(settings, "version", 1.0);
    cJSON_AddStringToObject(settings, "name", "example_project");
    cJSON *info = cJSON_AddObjectToObject(meta, "info");
    cJSON_AddStringToObject(info, "author", "tester");
    cJSON_AddNumberToObject(info, "id", 42.0);

    // step 3: Operate and Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *snap_ref = cJSON_CreateStringReference(buffer);
    cJSON_AddItemToObject(root, "snapshot", snap_ref);
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_AddItemToObject(settings, "detached_meta", detached_meta);
    cJSON *ver_item = cJSON_GetObjectItem(settings, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    (void)printed;
    (void)ver;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}