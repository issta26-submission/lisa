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
//<ID> 1626
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *s2 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(arr, s1);
    cJSON_AddNumberToObject(root, "ref_added_flag", (double)ref_added);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", nul);
    cJSON_bool is_null = cJSON_IsNull(nul);
    cJSON_AddNumberToObject(root, "maybe_null_flag", (double)is_null);
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "items_count", (double)count);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
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