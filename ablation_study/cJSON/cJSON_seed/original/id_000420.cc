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
//<ID> 420
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const int numbers[] = {10, 20, 30, 40};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *placeholder = cJSON_CreateString("will_be_replaced");
    cJSON_AddItemToObject(root, "to_replace", placeholder);
    cJSON_AddTrueToObject(root, "flag_set");

    // step 2: Configure
    cJSON *first_elem = cJSON_GetArrayItem(int_array, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *replacement_number = cJSON_CreateNumber(first_val * 1.5);

    // step 3: Operate
    cJSON_ReplaceItemInObject(root, "to_replace", replacement_number);
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free(snapshot);

    // step 4: Validate and Cleanup
    cJSON *replaced = cJSON_GetObjectItem(root, "to_replace");
    double replaced_val = cJSON_GetNumberValue(replaced);
    cJSON *validated = cJSON_CreateNumber(replaced_val + 2.0);
    cJSON_AddItemToObject(root, "validated_plus_two", validated);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}