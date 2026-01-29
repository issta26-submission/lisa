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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\": [true]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool inserted = cJSON_InsertItemInArray(arr, 1, false_item);
    cJSON_bool false_is_false = cJSON_IsFalse(false_item);
    cJSON_bool first_is_false = cJSON_IsFalse(first_elem);
    cJSON_AddNumberToObject(root, "flags_sum", (double)(inserted + false_is_false + first_is_false));

    // step 3: Operate
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}