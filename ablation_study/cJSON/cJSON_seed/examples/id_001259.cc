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
//<ID> 1259
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
    const char *ref_text = "hello_reference";
    cJSON *str_ref = cJSON_CreateStringReference(ref_text);

    // step 2: Configure
    cJSON_AddItemToArray(array, str_ref);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddStringToObject(root, "title", "example_title");
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    cJSON *detached_array = cJSON_DetachItemFromObjectCaseSensitive(root, "items");
    char *out_root = cJSON_PrintUnformatted(root);
    cJSON_free(out_root);
    char *out_detached = cJSON_PrintUnformatted(detached_array);
    cJSON_free(out_detached);

    // step 4: Cleanup
    cJSON_Delete(detached_array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}