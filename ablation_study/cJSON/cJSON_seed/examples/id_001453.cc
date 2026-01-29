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
//<ID> 1453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *bool_item = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, bool_item);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(arr);
    cJSON_bool arrays_equal = cJSON_Compare(arr, arr_dup, 1);
    cJSON *printed_str = cJSON_CreateString(printed);
    cJSON_AddItemToArray(arr_dup, printed_str);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(arr_dup);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    (void)arrays_equal;
    return 66;
}