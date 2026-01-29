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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int initial_numbers[4] = {1, 2, 3, 4};
    cJSON *numbers_arr = cJSON_CreateIntArray(initial_numbers, 4);
    cJSON *nullable = cJSON_CreateNull();
    cJSON *to_be_replaced = cJSON_CreateString("temporary_value");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "numbers", numbers_arr);
    cJSON_AddItemToObjectCS(root, "nullable", nullable);
    cJSON_AddItemToObjectCS(root, "replace_key", to_be_replaced);

    // step 3: Operate and Validate
    int replacement_numbers[3] = {10, 20, 30};
    cJSON *replacement_arr = cJSON_CreateIntArray(replacement_numbers, 3);
    cJSON_ReplaceItemViaPointer(root, to_be_replaced, replacement_arr);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}