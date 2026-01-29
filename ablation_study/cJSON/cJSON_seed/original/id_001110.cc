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
//<ID> 1110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old_name\",\"flag\":false,\"count\":42}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "new_name");
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate
    const char *current_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "copied_name", current_name);
    cJSON *copied = cJSON_GetObjectItem(root, "copied_name");
    cJSON_SetValuestring(copied, "copied_and_modified");

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}