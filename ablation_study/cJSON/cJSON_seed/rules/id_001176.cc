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
//<ID> 1176
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
    cJSON *first_item = NULL;
    cJSON *copied_string = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    const char *words[] = { "alpha", "beta", "gamma" };
    const char *retrieved = NULL;
    double set_result = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    strings_array = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", strings_array);
    first_item = cJSON_GetArrayItem(strings_array, 0);
    copied_string = cJSON_CreateString(cJSON_GetStringValue(first_item));
    cJSON_AddItemToObject(root, "first_copy", copied_string);
    num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", num_item);
    set_result = cJSON_SetNumberHelper(num_item, 3.0);

    // step 3: Operate / Validate
    retrieved = cJSON_GetStringValue(cJSON_GetObjectItem(root, "first_copy"));
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromArray(strings_array, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)retrieved;
    (void)set_result;
    return 66;
}