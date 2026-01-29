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
//<ID> 851
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *name_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_bool added_first = cJSON_AddItemToArray(array, true_item);
    (void)added_first;
    cJSON_AddItemToArray(array, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "boolean_array", array);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(array, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first_elem);
    (void)first_is_bool;
    char *snapshot = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}