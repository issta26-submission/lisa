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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *t1 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, t1);
    cJSON_AddItemToArray(array, f1);

    // step 2: Configure
    cJSON *inserted_true = cJSON_CreateTrue();
    cJSON_InsertItemInArray(array, 1, inserted_true);
    cJSON_AddItemToObject(root, "items", array);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "enabled");

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_item);
    cJSON *detached = cJSON_DetachItemFromArray(array, 2);
    cJSON_bool detached_is_bool = cJSON_IsBool(detached);
    char *printed = cJSON_PrintUnformatted(root);
    int summary = (int)first_is_bool + (int)detached_is_bool + (printed ? (int)printed[0] : 0) + root->type + meta->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}