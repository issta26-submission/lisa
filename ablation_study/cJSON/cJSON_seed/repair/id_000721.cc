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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, bfalse);

    // step 2: Configure
    cJSON *s2 = cJSON_CreateString("temp");
    char *set_ret = cJSON_SetValuestring(s2, "updated");
    (void)set_ret;
    cJSON_AddItemToArray(items, s2);

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}