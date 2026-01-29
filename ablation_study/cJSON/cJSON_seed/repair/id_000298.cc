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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"Alice\",\"active\":true,\"meta\":{\"id\":123}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_meta, "id", 456);

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_active = cJSON_IsTrue(active_item);
    cJSON_AddNumberToObject(root, "active_flag", (double)is_active);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_ref = cJSON_CreateStringReference(name_str);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_ReplaceItemViaPointer(root, meta_item, new_meta);
    cJSON_bool meta_is_obj = cJSON_IsObject(cJSON_GetObjectItem(root, "meta"));
    (void)meta_is_obj;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}