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
//<ID> 1581
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
    cJSON *settings = NULL;
    cJSON *arr = NULL;
    cJSON *first = NULL;
    cJSON *ref = NULL;
    char *out = NULL;
    const char *first_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "id", 42.0);
    cJSON_AddRawToObject(child, "meta", "{\"active\":true,\"score\":10}");
    cJSON_AddItemToObject(root, "child", child);
    settings = cJSON_AddObjectToObject(root, "settings");
    cJSON_AddStringToObject(settings, "mode", "fast");
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetStringValue(first);
    cJSON_AddStringToObject(settings, "first_item", first_val);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}