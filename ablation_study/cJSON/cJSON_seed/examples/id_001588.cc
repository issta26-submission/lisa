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
//<ID> 1588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = {"alpha", "beta", "gamma"};
    cJSON *array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", array);

    // step 2: Configure
    cJSON *extra = cJSON_CreateString("delta");
    cJSON_AddItemReferenceToArray(array, extra);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(array, 1);
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON *item_after = cJSON_GetArrayItem(array, 1);
    const char *retrieved = cJSON_GetStringValue(item_after);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)has_names;
    (void)retrieved;
    return 66;
}