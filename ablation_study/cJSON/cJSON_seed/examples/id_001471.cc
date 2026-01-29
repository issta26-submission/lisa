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
//<ID> 1471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":42,\"name\":\"test\",\"meta\":{\"a\":true}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *container = cJSON_CreateObject();

    // step 2: Configure
    cJSON_DeleteItemFromObjectCaseSensitive(dup, "meta");
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "name");
    cJSON_AddItemToObject(container, "detached_name", detached);

    // step 3: Operate & Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    char *s_root = cJSON_PrintUnformatted(root);
    char *s_dup = cJSON_PrintUnformatted(dup);
    char *s_container = cJSON_PrintUnformatted(container);
    cJSON_bool are_equal = cJSON_Compare(root, dup, 1);
    (void)root_is_obj;
    (void)are_equal;

    // step 4: Cleanup
    cJSON_free(s_root);
    cJSON_free(s_dup);
    cJSON_free(s_container);
    cJSON_Delete(container);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}