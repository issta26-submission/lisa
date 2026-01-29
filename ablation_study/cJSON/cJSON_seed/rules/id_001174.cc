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
//<ID> 1174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *str_array = NULL;
    cJSON *extra_str = NULL;
    cJSON *first_item = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    char *retrieved = NULL;
    double set_result = 0.0;
    const char *initial_strings[] = { "one", "two", "three" };

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(initial_strings, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    extra_str = cJSON_CreateString("zero");
    cJSON_AddItemToArray(str_array, extra_str);
    num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(str_array, 0);
    retrieved = cJSON_GetStringValue(first_item);
    (void)retrieved;
    set_result = cJSON_SetNumberHelper(num_item, 2.71828);
    (void)set_result;
    cJSON_DeleteItemFromArray(str_array, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}