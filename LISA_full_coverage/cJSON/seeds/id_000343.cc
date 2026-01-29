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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[1,2,3],\"meta\":{\"name\":\"test\"}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");

    // step 2: Configure
    cJSON *new_item = cJSON_CreateNumber(4.0);
    cJSON_bool added = cJSON_AddItemReferenceToArray(items, new_item);
    (void)added;

    // step 3: Operate
    int count_after = cJSON_GetArraySize(items);
    cJSON *last_item = cJSON_GetArrayItem(items, count_after - 1);
    char *printed = cJSON_Print(root);

    // step 4: Validate and Cleanup
    cJSON *detached = cJSON_DetachItemFromArray(items, count_after - 1);
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}