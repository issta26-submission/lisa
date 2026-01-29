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
//<ID> 1450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *array = cJSON_CreateArray();
    cJSON *str = cJSON_CreateString("alpha");
    cJSON *dup = cJSON_Duplicate(str, 1);
    cJSON *btrue = cJSON_CreateBool(1);
    cJSON_bool added1 = cJSON_AddItemToArray(array, str);
    cJSON_bool added2 = cJSON_AddItemToArray(array, dup);
    cJSON_bool added3 = cJSON_AddItemToArray(array, btrue);

    // step 2: Operate
    char *json_out = cJSON_PrintUnformatted(array);

    // step 3: Validate
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    cJSON_bool equal = cJSON_Compare(item0, item1, 1);
    (void)added1; (void)added2; (void)added3; (void)equal;

    // step 4: Cleanup
    cJSON_free(json_out);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}