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
//<ID> 1014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"meta\":{\"tag\":\"sample\"}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON_AddItemToObject(root, "detached_item", detached);
    cJSON *detached_id_item = cJSON_GetObjectItem(detached, "id");
    double id_value = cJSON_GetNumberValue(detached_id_item);
    cJSON *id_copy = cJSON_CreateNumber(id_value + 0.5);
    cJSON_AddItemToObject(root, "detached_id_plus", id_copy);

    // step 3: Operate & Validate
    cJSON *detached_name_item = cJSON_GetObjectItem(detached, "name");
    char *name_val = cJSON_GetStringValue(detached_name_item);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "detached_name_copy", name_copy);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *tag = cJSON_GetObjectItem(meta, "tag");
    char *tag_val = cJSON_GetStringValue(tag);
    cJSON *tag_copy = cJSON_CreateString(tag_val);
    cJSON_AddItemToObject(root, "meta_tag_copy", tag_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}