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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *list = cJSON_AddArrayToObject(root, "list");
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(list, false_item);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToArray(list, str_item);
    cJSON *inner = cJSON_AddObjectToObject(config, "inner");
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(inner, "answer", num);

    // step 3: Operate and Validate
    cJSON_bool was_bool = cJSON_IsBool(false_item);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(list, false_item, replacement);
    cJSON *first = cJSON_GetArrayItem(list, 0);
    const char *first_str = cJSON_GetStringValue(first);
    char *newvals = cJSON_SetValuestring(str_item, first_str);
    cJSON_SetNumberHelper(num, (double)was_bool + (double)replaced_ok + (newvals != NULL));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}