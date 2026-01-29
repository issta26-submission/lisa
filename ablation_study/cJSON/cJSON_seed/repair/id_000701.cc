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
//<ID> 701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *first = cJSON_CreateObject();
    cJSON_AddStringToObject(first, "name", "first");
    cJSON_AddNumberToObject(first, "value", 1.0);
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateObject();
    cJSON_AddStringToObject(second, "name", "second");
    cJSON_AddNumberToObject(second, "value", 2.0);

    // step 2: Configure
    cJSON_InsertItemInArray(items, 0, second);
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", 3.0);

    // step 3: Operate and Validate
    cJSON *elem0 = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(elem0, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    char *name_value = cJSON_GetStringValue(name_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)name_is_str;
    (void)name_value;
    (void)sum_item;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}