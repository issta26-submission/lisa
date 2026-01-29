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
//<ID> 480
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

    // step 2: Configure
    const char *lit = "reference-string";
    cJSON *ref = cJSON_CreateStringReference(lit);
    cJSON *owned = cJSON_CreateString("owned-string");
    cJSON_AddItemToArray(arr, ref);
    cJSON_AddItemToArray(arr, owned);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    char *first_val = cJSON_GetStringValue(first);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s-suffix", first_val ? first_val : "null");
    cJSON *replacement = cJSON_CreateString(buf);
    cJSON_ReplaceItemViaPointer(arr, first, replacement);
    char *new_first = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool are_equal = cJSON_Compare(root, parsed, 1);
    (void)are_equal;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}