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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {5, 10, 15};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *initial_value = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "replace_key", initial_value);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *replacement = cJSON_CreateNumber(first_val * 2.0);
    cJSON_ReplaceItemInObject(root, "replace_key", replacement);
    cJSON *derived = cJSON_CreateNumber(first_val + 3.0);
    cJSON_AddItemToObject(root, "derived_from_first", derived);

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(snapshot);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}