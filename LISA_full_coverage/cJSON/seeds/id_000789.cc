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
//<ID> 789
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "values");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":1}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, raw_item);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}