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
//<ID> 1117
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old\",\"value\":42}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *set_ret = cJSON_SetValuestring(name_item, "new_name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddStringToObject(root, "name_copy", name_str);

    // step 3: Operate
    cJSON_DeleteItemFromObject(root, "name");

    // step 4: Validate & Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}