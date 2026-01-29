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
//<ID> 281
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
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddTrueToObject(obj, "valid");
    cJSON_AddItemToObject(root, "obj", obj);
    /* Correct usage: add a reference to the existing 'obj' into 'arr' directly.
       Creating a separate reference object and then adding a reference-wrapper
       can lead to incorrect ownership/double-free issues. */
    cJSON_AddItemReferenceToArray(arr, obj);
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, bool_item);
    /* Similarly, add a reference to the existing 'arr' into 'root' under a new name. */
    cJSON_AddItemReferenceToObject(root, "numbers_ref", arr);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_bool = cJSON_IsBool(third);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    (void)first;
    (void)third_is_bool;

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}