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
//<ID> 1648
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
    cJSON *data = cJSON_AddObjectToObject(root, "data");

    // step 2: Configure
    cJSON *s_alpha = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "label", s_alpha);
    cJSON_AddStringToObject(data, "status", "ok");
    cJSON_AddStringToObject(root, "root_note", "root level");

    // step 3: Operate & Validate
    cJSON *status_item = cJSON_GetObjectItem(data, "status");
    const char *status_val = cJSON_GetStringValue(status_item);
    cJSON *status_copy = cJSON_CreateString(status_val);
    cJSON_AddItemToObject(meta, "status_copy", status_copy);
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}