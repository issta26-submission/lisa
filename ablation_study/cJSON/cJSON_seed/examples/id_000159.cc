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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("alice");
    cJSON_AddItemToObjectCS(info, "name", name);
    cJSON *status = cJSON_CreateString("active");
    cJSON_AddItemToObject(info, "status", status);

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(info, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *ref_name = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", ref_name);
    cJSON *got_status = cJSON_GetObjectItem(info, "status");
    char *status_val = cJSON_GetStringValue(got_status);
    cJSON *status_copy = cJSON_CreateString(status_val);
    cJSON_AddItemToObjectCS(root, "status_copy", status_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}