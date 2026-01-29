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
//<ID> 1622
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemReferenceToArray(arr, s2);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", nul);

    // step 2: Configure
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "initial_count", (double)count);
    cJSON_AddNumberToObject(root, "pi_approx", 3.14159);
    cJSON_bool is_nul = cJSON_IsNull(nul);
    cJSON_AddNumberToObject(root, "nothing_is_null", (double)is_nul);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    char *first_str = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}