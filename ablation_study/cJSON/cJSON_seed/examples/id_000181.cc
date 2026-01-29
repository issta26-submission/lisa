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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *external_ref = "external_string_ref";
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *ref_item = cJSON_CreateStringReference(external_ref);
    cJSON *copy_item = cJSON_CreateString("copied_string");
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(array, ref_item);
    cJSON_AddItemToArray(array, copy_item);
    cJSON_AddItemToArray(array, num_item);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("replaced_string");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(array, 1, replacement);
    (void)replaced_ok;
    cJSON *detached_array = cJSON_DetachItemFromObjectCaseSensitive(root, "items");
    char *printed_detached = cJSON_PrintUnformatted(detached_array);
    char *printed_root = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed_detached);
    cJSON_free(printed_root);
    cJSON_Delete(detached_array);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}