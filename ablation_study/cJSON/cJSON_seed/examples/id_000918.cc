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
//<ID> 918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("alpha");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(root, str1);
    cJSON_AddItemToArray(root, true_item);
    cJSON_AddItemToArray(root, num);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(str1, str2, 1);
    double new_number = cJSON_SetNumberHelper(num, 3.14 + (double)equal * 100.0);
    cJSON *retrieved = cJSON_GetArrayItem(root, 2);
    double retrieved_value = cJSON_GetNumberValue(retrieved);
    (void)new_number;
    (void)retrieved_value;

    // step 4: Cleanup
    cJSON_Delete(str2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}