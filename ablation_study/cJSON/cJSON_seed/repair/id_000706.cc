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
//<ID> 706
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *str_item = cJSON_CreateString("hello_world");
    cJSON_InsertItemInArray(items, 0, str_item);

    // step 2: Configure
    cJSON *num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    cJSON *fetched_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *first = cJSON_GetArrayItem(fetched_items, 0);
    cJSON_bool first_is_str = cJSON_IsString(first);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *ref = cJSON_CreateStringReference(first_val);
    cJSON_InsertItemInArray(items, 1, ref);

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}