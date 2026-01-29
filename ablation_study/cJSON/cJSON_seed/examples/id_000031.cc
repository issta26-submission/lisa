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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, btrue);
    char *first_name = cJSON_GetStringValue(s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "first_name", cJSON_CreateString(first_name));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("BETA_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON_bool is_true = cJSON_IsTrue(btrue);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    (void)replaced;
    (void)is_true;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}