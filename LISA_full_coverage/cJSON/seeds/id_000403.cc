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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *status_item = cJSON_AddStringToObject(root, "status", "ok");
    cJSON *list = cJSON_AddArrayToObject(root, "list");
    cJSON *elem0 = cJSON_CreateNumber(10.0);
    cJSON *elem1 = cJSON_CreateNumber(20.0);
    cJSON *elem2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(list, elem0);
    cJSON_AddItemToArray(list, elem1);
    cJSON_AddItemToArray(list, elem2);

    // step 2: Configure
    cJSON_bool was_invalid = cJSON_IsInvalid(status_item);
    (void)was_invalid;
    cJSON *new_count = cJSON_CreateNumber(42.0);

    // step 3: Operate
    cJSON_DeleteItemFromArray(list, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "count", new_count);

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}