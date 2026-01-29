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
//<ID> 840
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddStringToObject(root, "title", "Test Document");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "active");
    cJSON *note = cJSON_CreateString("primary user");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *name_value = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}