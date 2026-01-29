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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "doubles", darr);
    cJSON *cnt = cJSON_AddNumberToObject(root, "count", 3.0);
    (void)cnt;
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(list, num_item);

    // step 3: Operate & Validate
    int doubles_size = cJSON_GetArraySize(darr);
    cJSON_AddNumberToObject(root, "doubles_size", (double)doubles_size);
    cJSON *first_elem = cJSON_GetArrayItem(darr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON_AddNumberToObject(root, "first_double", first_val);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}