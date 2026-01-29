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
//<ID> 36
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    char *updated = cJSON_SetValuestring(s1, "alpha_updated");

    // step 3: Operate and Validate
    char *before = cJSON_PrintUnformatted(root);
    int count = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_string = cJSON_IsString(first);
    double validation_value = (double)count + (double)is_string;
    cJSON_AddNumberToObject(root, "validation", validation_value);
    char *after = cJSON_PrintUnformatted(root);
    cJSON_free(before);
    cJSON_free(after);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)updated;
    return 66; // API sequence test completed successfully
}