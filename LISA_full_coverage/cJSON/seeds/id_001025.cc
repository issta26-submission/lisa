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
//<ID> 1025
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *num_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON *label_item = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "label", label_item);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(num_array, 1);
    cJSON_AddItemToObject(root, "numbers_dup", dup_array);

    // step 3: Operate
    char *label_value = cJSON_GetStringValue(label_item);
    cJSON_AddStringToObject(root, "label_copy", label_value);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}