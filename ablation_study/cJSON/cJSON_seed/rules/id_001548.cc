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
//<ID> 1548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *arr_dup = NULL;
    cJSON *arr_ref = NULL;
    cJSON *str0 = NULL;
    char *out = NULL;
    const char *greeting_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str0 = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str0);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "list", arr);

    // create a separate duplicate for the reference to avoid double-free when deleting the root
    arr_dup = cJSON_Duplicate(arr, 1);
    arr_ref = cJSON_CreateArrayReference(arr_dup);
    cJSON_AddItemToObject(root, "list_ref", arr_ref);

    // step 3: Operate / Validate
    cJSON_AddTrueToObject(root, "ok");
    greeting_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    cJSON_AddItemToObject(root, "greeting_copy", cJSON_CreateString(greeting_val));
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}