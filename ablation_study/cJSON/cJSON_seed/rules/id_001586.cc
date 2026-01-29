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
//<ID> 1586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *elem0 = NULL;
    char *out = NULL;
    const char *first_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddRawToObject(child, "raw", "{\"k\":123}");
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 3: Operate / Validate
    elem0 = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetStringValue(elem0);
    cJSON_AddStringToObject(root, "first_copy", first_val);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}