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
//<ID> 674
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, true_item);
    cJSON_AddItemToArray(items, str2);

    // step 2: Configure
    cJSON *status = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "status", status);
    cJSON *newstatus = cJSON_CreateString("updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "status", newstatus);

    // step 3: Operate
    cJSON *item0 = cJSON_GetArrayItem(items, 0);
    cJSON *item1 = cJSON_GetArrayItem(items, 1);
    cJSON_bool first_is_string = cJSON_IsString(item0);
    cJSON_bool second_is_bool = cJSON_IsBool(item1);
    cJSON_AddBoolToObject(meta, "first_is_string", first_is_string);
    cJSON_AddBoolToObject(meta, "second_is_bool", second_is_bool);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(meta, "items_count", (double)count);

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    (void)replaced;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}