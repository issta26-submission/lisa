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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "items");
    cJSON *first_str = cJSON_CreateString("initial");
    cJSON *second_str = cJSON_CreateString("to_be_changed");

    // step 2: Configure
    cJSON_AddItemToArray(array, first_str);
    cJSON_SetValuestring(second_str, "modified");
    cJSON_AddItemToArray(array, second_str);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(array);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON_bool item0_is_string = cJSON_IsString(item0);
    cJSON_AddNumberToObject(root, "validation", (double)(arr_size + (int)item0_is_string));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}