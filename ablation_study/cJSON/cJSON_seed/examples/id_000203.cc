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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *nil_item = cJSON_CreateNull();
    cJSON_AddItemToObject(container, "empty", nil_item);
    cJSON *orig_ref = cJSON_CreateStringReference("original_value");
    cJSON_AddItemToObject(container, "label", orig_ref);

    // step 2: Configure
    cJSON *replacement_ref = cJSON_CreateStringReference("replaced_value");
    cJSON_bool replace_result = cJSON_ReplaceItemViaPointer(container, orig_ref, replacement_ref);
    cJSON_bool was_null = cJSON_IsNull(nil_item);

    // step 3: Operate & Validate
    cJSON *fetched = cJSON_GetObjectItem(container, "label");
    const char *fetched_str = cJSON_GetStringValue(fetched);
    (void)fetched_str;
    (void)replace_result;
    (void)was_null;
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}