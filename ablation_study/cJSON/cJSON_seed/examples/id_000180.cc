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
//<ID> 180
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
    cJSON_AddItemToObject(root, "list", array);
    cJSON *elem0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, elem0);
    cJSON *elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, elem1);
    cJSON *ref_elem = cJSON_CreateStringReference("gamma_ref");
    cJSON_AddItemToArray(array, ref_elem);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("beta_replacement");
    cJSON_ReplaceItemInArray(array, 1, replacement);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(array);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "list");
    cJSON_AddItemToObject(root, "detached_list", detached);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}