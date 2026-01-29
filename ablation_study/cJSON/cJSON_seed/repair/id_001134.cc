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
//<ID> 1134
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
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    char *setres = cJSON_SetValuestring(str_item, "modified");
    cJSON *flag_item = cJSON_AddTrueToObject(root, "flag");
    cJSON *extra_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, extra_true);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_str = cJSON_GetStringValue(first);
    double computed_val = (double)arr_size * 2.5 + (first_str ? 1.0 : 0.0);
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}