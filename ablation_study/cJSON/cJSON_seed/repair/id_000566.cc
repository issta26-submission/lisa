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
//<ID> 566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {10.5, 20.25, -5.75};
    cJSON *nums = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON_AddStringToObject(root, "label", "double_array");

    // step 2: Configure
    cJSON *fetched_nums = cJSON_GetObjectItemCaseSensitive(root, "nums");
    cJSON *elem0 = cJSON_GetArrayItem(fetched_nums, 0);
    double v0 = cJSON_GetNumberValue(elem0);
    cJSON *elem1 = cJSON_GetArrayItem(fetched_nums, 1);
    double v1 = cJSON_GetNumberValue(elem1);
    cJSON *elem2 = cJSON_GetArrayItem(fetched_nums, 2);
    double v2 = cJSON_GetNumberValue(elem2);
    double total = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", total);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(fetched_nums, 0);
    cJSON *new_first = cJSON_GetArrayItem(fetched_nums, 0);
    double new_first_val = cJSON_GetNumberValue(new_first);
    cJSON_AddNumberToObject(root, "new_first", new_first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}