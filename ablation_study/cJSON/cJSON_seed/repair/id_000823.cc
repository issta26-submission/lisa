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
//<ID> 823
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
    cJSON_AddNumberToObject(meta, "version", 1.23);
    cJSON *explicit_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "explicit", explicit_num);

    // step 2: Configure
    const char *json_text = "{\"name\":\"example\",\"unused\":\"to_remove\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    (void)name_is_string;
    cJSON_AddNumberToObject(parsed, "added_number", 99.9);

    // step 3: Operate and Validate
    cJSON *ptr_explicit = cJSON_GetObjectItem(root, "explicit");
    cJSON *detached_via_ptr = cJSON_DetachItemViaPointer(root, ptr_explicit);
    cJSON *detached_from_parsed = cJSON_DetachItemFromObject(parsed, "unused");
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    (void)snapshot_root;
    (void)snapshot_parsed;

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(detached_via_ptr);
    cJSON_Delete(detached_from_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}