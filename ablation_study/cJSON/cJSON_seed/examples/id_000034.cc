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
//<ID> 34
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
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *s3 = cJSON_CreateString("third");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, s3);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("SECOND_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *fetched_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(fetched_flag);

    // step 3: Operate & Validate
    cJSON *replaced_item = cJSON_GetArrayItem(array, 1);
    char *replaced_str = cJSON_GetStringValue(replaced_item);
    cJSON *name_item = cJSON_CreateString(replaced_str);
    cJSON_AddItemToObject(root, "replaced_name", name_item);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)replaced;
    (void)flag_is_true;
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}