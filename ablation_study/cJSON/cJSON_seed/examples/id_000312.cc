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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int init_numbers[] = {1, 2, 3};
    cJSON *int_array = cJSON_CreateIntArray(init_numbers, 3);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *pi_item = cJSON_CreateNumber(3.14);

    // step 2: Configure
    cJSON_AddItemToArray(int_array, pi_item);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate & Validate
    cJSON *second_elem = cJSON_GetArrayItem(int_array, 1);
    double second_value = cJSON_GetNumberValue(second_elem);
    cJSON *adjusted = cJSON_CreateNumber(second_value + 0.5);
    cJSON_AddItemToArray(int_array, adjusted);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_AddItemToObject(root, "snapshot", cJSON_CreateString(snapshot));
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}