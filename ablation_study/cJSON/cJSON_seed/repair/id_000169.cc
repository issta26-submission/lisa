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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *group = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "group", group);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(group, "nullable", nullable);

    // step 2: Configure
    int *numbers = (int*)cJSON_malloc(3 * sizeof(int));
    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;
    cJSON *num_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(group, "values", num_array);

    // step 3: Operate and Validate
    cJSON *group_copy = cJSON_Duplicate(group, 1);
    cJSON_AddItemToObject(root, "group_copy", group_copy);
    cJSON *replacement = cJSON_CreateString("was_null_replaced");
    cJSON_ReplaceItemViaPointer(group, nullable, replacement);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(numbers);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}