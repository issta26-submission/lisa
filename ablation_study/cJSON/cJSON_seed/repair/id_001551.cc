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
//<ID> 1551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"name\":\"parsedName\",\"flag\":\"yes\"}");
    const cJSON *orig_name = cJSON_GetObjectItem(parsed, "name");
    cJSON *dup_name = cJSON_Duplicate(orig_name, 1);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(root, "name", dup_name);
    cJSON_AddItemToObjectCS(root, "count", count);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replacedName");
    cJSON_ReplaceItemViaPointer(root, dup_name, replacement);
    cJSON *is_str_bool = cJSON_CreateBool(cJSON_IsString(replacement));
    cJSON_AddItemToObjectCS(root, "name_is_string", is_str_bool);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_is_string_flag = cJSON_IsString(name_item);
    cJSON *validated = cJSON_CreateBool(name_is_string_flag);
    cJSON_AddItemToObjectCS(root, "validated", validated);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}