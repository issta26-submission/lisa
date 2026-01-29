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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_array = NULL;
    cJSON *copied_str = NULL;
    cJSON *first_item = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    const char *strings[] = { "alpha", "beta", "gamma" };
    double set_result = 0.0;

    // step 2: Setup / Configure
    str_array = cJSON_CreateStringArray(strings, 3);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "strings", str_array);
    first_item = cJSON_GetArrayItem(str_array, 0);
    copied_str = cJSON_CreateString(cJSON_GetStringValue(first_item));
    cJSON_AddItemToArray(str_array, copied_str);
    num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "value", num_item);
    set_result = cJSON_SetNumberHelper(num_item, 2026.0);

    // step 3: Operate / Validate
    cJSON_DeleteItemFromArray(str_array, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)set_result;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}