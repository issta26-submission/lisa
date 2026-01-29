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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", double_array);

    // step 2: Configure
    cJSON *retrieved = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first_elem = cJSON_GetArrayItem(retrieved, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *scaled = cJSON_CreateNumber(first_val * 10.0);
    cJSON_AddItemToObject(root, "scaled_first", scaled);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(retrieved, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}