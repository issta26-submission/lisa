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
//<ID> 1172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *strings_array = NULL;
    cJSON *first_str_item = NULL;
    cJSON *copied_string = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    char *extracted = NULL;
    double set_result = 0.0;
    int size_before = 0;
    int size_after = 0;
    const char *string_list[] = { "alpha", "beta", "gamma" };

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    strings_array = cJSON_CreateStringArray(string_list, 3);
    cJSON_AddItemToObject(root, "words", strings_array);
    num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 3: Operate / Validate
    first_str_item = cJSON_GetArrayItem(strings_array, 0);
    extracted = cJSON_GetStringValue(first_str_item);
    copied_string = cJSON_CreateString(extracted);
    cJSON_AddItemToObject(root, "copied_first", copied_string);
    size_before = cJSON_GetArraySize(strings_array);
    set_result = cJSON_SetNumberHelper(num_item, 3.14159);
    cJSON_DeleteItemFromArray(strings_array, 1);
    size_after = cJSON_GetArraySize(strings_array);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}