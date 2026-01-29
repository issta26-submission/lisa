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
//<ID> 792
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *entry0 = cJSON_CreateObject();
    cJSON_AddTrueToObject(entry0, "is_first");
    cJSON_AddItemToArray(array, entry0);
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddFalseToObject(entry1, "is_second");
    cJSON_AddItemToArray(array, entry1);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(array, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second_item);
    cJSON_AddNumberToObject(root, "second_invalid_flag", (double)second_invalid);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}