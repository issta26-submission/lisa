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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *oldstr = cJSON_CreateString("old_value");

    // step 2: Configure
    cJSON_AddItemToArray(extras, false_item);
    cJSON_AddItemToObject(nested, "key", oldstr);
    cJSON *new_false = cJSON_CreateFalse();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(nested, oldstr, new_false);

    // step 3: Operate and Validate
    cJSON *replaced_child = cJSON_GetObjectItem(nested, "key");
    cJSON_bool is_bool = cJSON_IsBool(replaced_child);
    int arr_size = cJSON_GetArraySize(extras);
    int validation_value = (int)is_bool + arr_size + (int)replaced;
    cJSON_AddNumberToObject(root, "validation", (double)validation_value);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}