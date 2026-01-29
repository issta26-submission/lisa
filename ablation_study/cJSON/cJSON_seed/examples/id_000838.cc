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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = { 1.0, 2.0, 3.0 };
    cJSON *num_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *replacement_raw = cJSON_CreateRaw("REPLACED_RAW");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "numbers", num_array);
    cJSON_AddItemToObjectCS(root, "initial_raw", cJSON_CreateRaw("INITIAL_RAW"));

    // step 3: Operate & Validate
    cJSON_ReplaceItemInArray(num_array, 1, replacement_raw);
    cJSON *replaced_item = cJSON_GetArrayItem(num_array, 1);
    cJSON_bool replaced_is_raw = cJSON_IsRaw(replaced_item);
    cJSON *validation_flag = cJSON_CreateNumber((double)replaced_is_raw);
    cJSON_AddItemToObjectCS(root, "replaced_is_raw", validation_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}