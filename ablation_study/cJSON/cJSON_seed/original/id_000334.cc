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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int base_nums[] = {10, 20, 30};
    cJSON *numbers = cJSON_CreateIntArray(base_nums, 3);
    cJSON_AddItemToObjectCS(root, "numbers", numbers);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "maybe_null", nullable);

    // step 2: Configure
    int replacement_nums[] = {100, 200};
    cJSON *replacement = cJSON_CreateIntArray(replacement_nums, 2);
    cJSON *orig = cJSON_GetObjectItem(root, "numbers");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, orig, replacement);

    // step 3: Operate
    cJSON *first_in_replacement = cJSON_GetArrayItem(replacement, 0);
    double first_value = cJSON_GetNumberValue(first_in_replacement);
    cJSON *derived = cJSON_CreateNumber(first_value + 1.0);
    cJSON_AddItemToObjectCS(root, "derived_from_first", derived);

    // step 4: Validate and Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}