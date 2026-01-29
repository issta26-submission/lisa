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
//<ID> 1302
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
    cJSON *after_first = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    cJSON_bool first_was_true = 0;
    cJSON_bool detached_is_true = 0;
    cJSON_bool after_first_is_true = 0;

    // step 2: Setup / Configure
    const char *json_text = "{\"arr\":[true,false,123,\"abc\"]}";
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, "arr");
    first_item = cJSON_GetArrayItem(arr, 0);
    first_was_true = cJSON_IsTrue(first_item);
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemViaPointer(arr, first_item);
    detached_is_true = cJSON_IsTrue(detached);
    after_first = cJSON_GetArrayItem(arr, 0);
    after_first_is_true = cJSON_IsTrue(after_first);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}