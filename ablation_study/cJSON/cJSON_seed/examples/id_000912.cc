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
//<ID> 912
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("value-1");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToArray(root, str_item);
    cJSON_AddItemToArray(root, true_item);
    cJSON_AddItemToArray(root, num_item);

    // step 2: Configure
    double initial_set = cJSON_SetNumberHelper(num_item, 100.5);
    (void)initial_set;

    // step 3: Operate & Validate
    cJSON *fetched_str = cJSON_GetArrayItem(root, 0);
    cJSON *fetched_num = cJSON_GetArrayItem(root, 2);
    cJSON *str_copy = cJSON_CreateString(cJSON_GetStringValue(fetched_str));
    cJSON_bool are_equal = cJSON_Compare(fetched_str, str_copy, 1);
    double adjusted = cJSON_SetNumberHelper(fetched_num, cJSON_SetNumberHelper(fetched_num, 0.0) + (double)are_equal + 3.0);
    (void)adjusted;

    // step 4: Cleanup
    cJSON_Delete(str_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}