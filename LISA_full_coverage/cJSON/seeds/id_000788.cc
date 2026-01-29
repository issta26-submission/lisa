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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "items");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *number_item = cJSON_CreateNumber(3.14159);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");

    // step 2: Configure
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, number_item);
    cJSON_AddItemToArray(array, raw_item);
    cJSON *count_item = cJSON_CreateNumber((double)cJSON_GetArraySize(array));
    cJSON_AddItemToObject(root, "count", count_item);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}