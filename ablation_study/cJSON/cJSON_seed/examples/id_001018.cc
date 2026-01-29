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
//<ID> 1018
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    const char json[] = "{\"items\":[{\"id\":1,\"name\":\"one\"},{\"id\":2,\"name\":\"two\"}],\"meta\":{\"tag\":\"sample\"}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "extra", extra);

    // step 2: Operate
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON *detached_name = cJSON_GetObjectItem(detached, "name");
    char *detached_name_str = cJSON_GetStringValue(detached_name);
    cJSON *name_copy = cJSON_CreateString(detached_name_str);
    cJSON_AddItemToObject(extra, "extracted_name", name_copy);

    // step 3: Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *id = cJSON_GetObjectItem(first, "id");
    double id_val = cJSON_GetNumberValue(id);
    cJSON *id_copy = cJSON_CreateNumber(id_val);
    cJSON_AddItemToObject(root, "first_id_copy", id_copy);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *tag = cJSON_GetObjectItem(meta, "tag");
    char *tag_str = cJSON_GetStringValue(tag);
    cJSON *tag_copy = cJSON_CreateString(tag_str);
    cJSON_AddItemToObject(root, "meta_tag_copy", tag_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}