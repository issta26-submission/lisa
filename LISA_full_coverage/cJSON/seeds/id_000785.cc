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
//<ID> 785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(3.14159265);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":1}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON *size_item = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "items_count", size_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}