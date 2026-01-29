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
//<ID> 1455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON *bool_item = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_bool added_str = cJSON_AddItemToArray(array, str_item);
    cJSON_bool added_bool = cJSON_AddItemToArray(array, bool_item);

    // step 3: Operate & Validate
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_bool equal_structure = cJSON_Compare(array, dup_array, 1);
    char *printed_orig = cJSON_PrintUnformatted(array);
    char *printed_dup = cJSON_PrintUnformatted(dup_array);

    // step 4: Cleanup
    cJSON_free(printed_orig);
    cJSON_free(printed_dup);
    cJSON_Delete(dup_array);
    cJSON_Delete(array);
    (void)added_str;
    (void)added_bool;
    (void)equal_structure;

    // API sequence test completed successfully
    return 66;
}