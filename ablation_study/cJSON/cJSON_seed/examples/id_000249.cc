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
//<ID> 249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14159);
    cJSON *n2 = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    double set_result = cJSON_SetNumberHelper(n2, 42.0);
    (void)set_result;
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "answer", n2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *first_copy = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *count_num = cJSON_CreateNumber((double)cJSON_GetArraySize(arr));
    cJSON_AddItemToObject(root, "count", count_num);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}