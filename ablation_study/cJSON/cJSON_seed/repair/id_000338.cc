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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig_array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(orig_array, num1);
    cJSON_AddItemToArray(orig_array, num2);
    cJSON_AddItemToObject(root, "items", orig_array);

    // step 2: Configure
    cJSON *new_array = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(new_array, s1);
    cJSON_AddItemToArray(new_array, s2);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "items", new_array);

    // step 3: Operate and Validate
    cJSON *items_after = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items_after, 0);
    char *out_root = cJSON_Print(root);
    char *out_first = cJSON_Print(first_item);

    // step 4: Cleanup
    cJSON_free(out_first);
    cJSON_free(out_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}