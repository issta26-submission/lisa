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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON *item0 = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    replaced = cJSON_ReplaceItemViaPointer(arr, item0, replacement);
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)size_after);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}