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
//<ID> 825
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
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, num1);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, num2);

    // step 2: Configure
    const char *json_text = "{\"name\":\"example\",\"remove_me\":123}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");

    // step 3: Operate and Validate
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    (void)name_is_str;
    cJSON *detached_from_parsed = cJSON_DetachItemFromObject(parsed, "remove_me");
    cJSON_AddItemToObject(root, "imported_num", detached_from_parsed);
    cJSON *detached_via_ptr = cJSON_DetachItemViaPointer(items, num1);
    cJSON_AddItemToObject(meta, "moved", detached_via_ptr);
    cJSON *created_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "created", created_num);
    cJSON *also_added = cJSON_AddNumberToObject(root, "direct", 7.5);
    (void)also_added;

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}