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
//<ID> 725
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
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    char *old_val = cJSON_SetValuestring(s2, "gamma");
    (void)old_val;
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, flag_false);

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    int total = cJSON_GetArraySize(items_ref);
    cJSON_AddNumberToObject(meta, "count", (double)total);
    cJSON *second = cJSON_GetArrayItem(items_ref, 1);
    char *second_str = cJSON_GetStringValue(second);
    cJSON_AddStringToObject(meta, "second_name", second_str ? second_str : ""); 
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}