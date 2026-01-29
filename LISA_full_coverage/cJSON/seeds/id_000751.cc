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
//<ID> 751
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"meta\":{\"tag\":\"orig\"},\"value\":\"old\"}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":\"rawval\"}");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    char *old_value_str = cJSON_SetValuestring(cJSON_GetObjectItem(root, "value"), "new");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");

    // step 3: Operate
    cJSON_AddItemToObject(root, "meta_backup", detached_meta);
    char *buffered = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate & Cleanup
    // Do not free old_value_str here: it is owned by the cJSON structure and will be freed by cJSON_Delete(root).
    cJSON_free(buffered);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)old_value_str; // keep variable used as in original logic
    return 66;
}