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
//<ID> 1306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *arr_item = NULL;
    cJSON *detached_item = NULL;
    char *printed = NULL;
    cJSON_bool was_true = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse("[ true, 42, \"sample\" ]");
    obj = cJSON_CreateObject();

    // step 3: Operate / Validate
    arr_item = cJSON_GetArrayItem(root, 0);
    was_true = cJSON_IsTrue(arr_item);
    detached_item = cJSON_DetachItemViaPointer(root, arr_item);
    cJSON_AddItemToObject(obj, "moved_item", detached_item);
    cJSON_AddBoolToObject(obj, "original_was_true", was_true);
    printed = cJSON_PrintUnformatted(obj);
    (void)printed;
    (void)was_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(obj);
    // API sequence test completed successfully
    return 66;
}