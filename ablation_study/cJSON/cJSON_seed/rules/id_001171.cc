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
//<ID> 1171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *first_item = NULL;
    cJSON *copied_string = NULL;
    cJSON *extra_string = NULL;
    cJSON *number_item = NULL;
    char *printed = NULL;
    const char *string_array[] = { "alpha", "beta", "gamma" };
    const char *first_value = NULL;
    double set_result = 0.0;

    // step 2: Setup / Configure
    arr = cJSON_CreateStringArray(string_array, 3);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "strings", arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(first_item);
    copied_string = cJSON_CreateString(first_value);
    cJSON_AddItemToObject(root, "first_copy", copied_string);
    extra_string = cJSON_CreateString("extra");
    cJSON_AddItemToArray(arr, extra_string);
    number_item = cJSON_CreateNumber(0.0);
    set_result = cJSON_SetNumberHelper(number_item, 2026.0);
    cJSON_AddItemToObject(root, "year", number_item);

    // step 3: Operate / Validate
    cJSON_DeleteItemFromArray(arr, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)set_result;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}