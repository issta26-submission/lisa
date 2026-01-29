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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", int_array);

    // step 2: Configure
    cJSON *extra = cJSON_CreateArray();
    cJSON *e0 = cJSON_CreateNumber(10.0);
    cJSON *e1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(extra, e0);
    cJSON_AddItemToArray(extra, e1);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *first_int = cJSON_GetArrayItem(int_array, 0);
    cJSON *first_extra = cJSON_GetArrayItem(extra, 0);
    cJSON_bool equal = cJSON_Compare(first_int, first_extra, 1);
    cJSON_AddBoolToObject(root, "first_equal_extra_first", equal);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(int_array, 1);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}