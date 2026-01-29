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
//<ID> 1454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *expected = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON *str_item_exp = cJSON_CreateString("example");
    cJSON *bool_item_exp = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToArray(array, bool_item);
    cJSON_AddItemToArray(expected, str_item_exp);
    cJSON_AddItemToArray(expected, bool_item_exp);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(array);
    cJSON_bool printed_nonnull = (printed != (char*)0);
    cJSON_bool sizes_match = (cJSON_GetArraySize(array) == cJSON_GetArraySize(expected));
    cJSON_bool equal = cJSON_Compare(array, expected, 1);
    (void)printed_nonnull;
    (void)sizes_match;
    (void)equal;

    // step 4: Cleanup
    if (printed) { cJSON_free(printed); } /* single conditional only for safe free; not used for control flow */
    cJSON_Delete(array);
    cJSON_Delete(expected);

    // API sequence test completed successfully
    return 66;
}