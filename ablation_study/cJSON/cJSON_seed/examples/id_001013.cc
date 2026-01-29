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
//<ID> 1013
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"meta\":{\"name\":\"sensor-1\",\"value\":42.5},\"items\":[\"alpha\",\"beta\",\"gamma\"],\"count\":3}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON_AddItemToObject(root, "detached_item", detached);
    cJSON *computed_num = cJSON_CreateNumber(cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count")) + 1.0);
    cJSON_AddItemToObject(root, "computed", computed_num);

    // step 3: Operate & Validate
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}