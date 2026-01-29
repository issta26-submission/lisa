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
//<ID> 1458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *array = cJSON_CreateArray();
    cJSON *str_a = cJSON_CreateString("example");
    cJSON *str_b = cJSON_CreateString("example");
    cJSON *bool_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(array, str_a);
    cJSON_AddItemToArray(array, bool_true);

    // step 2: Operate
    char *out_before = cJSON_PrintUnformatted(array);

    // step 3: Validate & Update
    cJSON_bool strings_equal = cJSON_Compare(str_a, str_b, 1);
    cJSON *comp_bool = cJSON_CreateBool(strings_equal);
    cJSON_AddItemToArray(array, comp_bool);
    char *out_after = cJSON_PrintUnformatted(array);

    // step 4: Cleanup
    cJSON_free(out_before);
    cJSON_free(out_after);
    cJSON_Delete(array);
    cJSON_Delete(str_b);

    // API sequence test completed successfully
    return 66;
}