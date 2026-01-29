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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "data");
    cJSON *item_true = cJSON_CreateTrue();
    cJSON *item_num = cJSON_CreateNumber(3.14159);
    cJSON *item_raw = cJSON_CreateRaw("{\"nested\":true}");

    // step 2: Configure
    cJSON_AddItemToArray(array, item_true);
    cJSON_AddItemToArray(array, item_num);
    cJSON_AddItemToArray(array, item_raw);

    // step 3: Operate
    int size = cJSON_GetArraySize(array);
    cJSON *count_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}