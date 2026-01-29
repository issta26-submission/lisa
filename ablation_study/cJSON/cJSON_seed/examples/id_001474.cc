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
//<ID> 1474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":1,\"b\":{\"nested\":2},\"c\":\"text\"}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *root_dup = cJSON_Duplicate(root, 1);
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToObject(root, "d", extra);

    // step 2: Configure
    char *printed_root = cJSON_PrintUnformatted(root);
    char *printed_dup = cJSON_PrintUnformatted(root_dup);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool equal_before = cJSON_Compare(root, root_dup, 1);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "b");
    cJSON_bool detached_is_obj = cJSON_IsObject(detached);
    char *printed_detached = cJSON_PrintUnformatted(detached);
    cJSON_DeleteItemFromObjectCaseSensitive(root_dup, "c");
    char *printed_dup_after = cJSON_PrintUnformatted(root_dup);
    cJSON_bool equal_after = cJSON_Compare(root, root_dup, 1);

    // step 4: Cleanup
    (void)root_is_obj;
    (void)equal_before;
    (void)detached_is_obj;
    (void)equal_after;
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(printed_detached);
    cJSON_free(printed_dup_after);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_Delete(root_dup);

    // API sequence test completed successfully
    return 66;
}