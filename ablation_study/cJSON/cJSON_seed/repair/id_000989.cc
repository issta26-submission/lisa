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
//<ID> 989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *entries = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "entries", entries);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(entries, first);
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(entries, second);
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 3.14159);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(entries, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON *copied = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "copied_second", copied);
    cJSON *replacement = cJSON_CreateString("replacement_alpha");
    cJSON_ReplaceItemInArray(entries, 0, replacement);
    char *copied_value = cJSON_GetStringValue(copied);
    cJSON *copied_again = cJSON_CreateString(copied_value);
    cJSON_AddItemToObject(root, "copied_again", copied_again);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}