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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *list = cJSON_AddArrayToObject(root, "list");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(list, false_item);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced-value");

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(list, 0);
    cJSON_ReplaceItemViaPointer(list, first, replacement);
    cJSON_bool is_bool = cJSON_IsBool(replacement);
    cJSON_AddNumberToObject(meta, "replacement_was_bool", (double)is_bool);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}