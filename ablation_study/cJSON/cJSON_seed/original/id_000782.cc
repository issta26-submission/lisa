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
//<ID> 782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "arr");

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, true_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"inserted\":true}");
    cJSON_AddItemToArray(array, raw_item);

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(array);
    cJSON *size_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", size_item);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}