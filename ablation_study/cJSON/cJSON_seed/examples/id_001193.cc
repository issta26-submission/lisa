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
//<ID> 1193
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
    cJSON *first_true = cJSON_CreateTrue();
    cJSON *replacement_true = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, first_true);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate & Validate
    cJSON_ReplaceItemInArray(arr, 0, replacement_true);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON_bool are_equal = cJSON_Compare(item0, replacement_true, 1);
    cJSON_AddNumberToObject(root, "replacement_matches", (double)are_equal);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}