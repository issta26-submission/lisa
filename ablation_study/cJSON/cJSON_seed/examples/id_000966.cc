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
//<ID> 966
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-abc");
    cJSON *desc = cJSON_CreateString("initial description");

    // step 2: Configure
    cJSON_AddItemToObject(metadata, "name", name);
    cJSON_AddItemToObject(metadata, "desc", desc);
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    char *set_ret = cJSON_SetValuestring(desc, "updated description");
    const char *got_name = cJSON_GetStringValue(name);
    const char *got_desc = cJSON_GetStringValue(desc);
    cJSON_AddItemToObject(root, "extracted_name", cJSON_CreateString(got_name));
    cJSON_AddItemToObject(root, "extracted_desc", cJSON_CreateString(got_desc));
    cJSON_AddItemToObject(root, "set_result", cJSON_CreateString(set_ret));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}