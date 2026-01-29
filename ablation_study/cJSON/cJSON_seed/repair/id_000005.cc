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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *bool_node = NULL;
    cJSON_bool is_true = 0;
    char *updated = NULL;
    char tmpbuf[32];
    memset(tmpbuf, 0, sizeof(tmpbuf));

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "name", str_item);
    bool_node = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate
    updated = cJSON_SetValuestring(str_item, "updated");
    (void)updated;

    // step 4: Validate & Cleanup
    is_true = cJSON_IsBool(true_item);
    (void)is_true;
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}