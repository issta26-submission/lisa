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
//<ID> 1137
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
    cJSON *item_alpha = NULL;
    cJSON *ref_beta = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *repl = NULL;
    cJSON *meta = NULL;
    cJSON *new_meta = NULL;
    cJSON *first_copy = NULL;
    char *printed = NULL;
    const char *alpha = "alpha";
    const char *beta = "beta";
    const char *first_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item_alpha = cJSON_CreateString(alpha);
    cJSON_AddItemToArray(arr, item_alpha);
    ref_beta = cJSON_CreateStringReference(beta);
    cJSON_AddItemReferenceToArray(arr, ref_beta);
    meta = cJSON_CreateString("old_meta");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    first_val = cJSON_GetStringValue(got0);
    first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    repl = cJSON_CreateString("replaced_beta");
    cJSON_ReplaceItemInArray(arr, 1, repl);
    new_meta = cJSON_CreateString("new_meta");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}