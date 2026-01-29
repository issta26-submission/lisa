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
//<ID> 1625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *refs = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "references", refs);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", null_item);

    // step 2: Configure
    int size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)size);
    cJSON_AddItemReferenceToArray(refs, s1);
    cJSON_AddItemReferenceToArray(refs, s2);
    cJSON_AddNumberToObject(root, "refs_count", (double)cJSON_GetArraySize(refs));

    // step 3: Operate & Validate
    cJSON_bool is_null = cJSON_IsNull(null_item);
    cJSON_AddNumberToObject(root, "null_flag", (double)is_null);
    char *first_val = cJSON_GetStringValue(s1);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}