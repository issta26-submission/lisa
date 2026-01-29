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
//<ID> 740
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *replacement = NULL;
    cJSON *got_alpha_before = NULL;
    cJSON *got_alpha_after = NULL;
    cJSON_bool equal_before = 0;
    cJSON_bool replaced = 0;
    cJSON_bool equal_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    item1 = cJSON_CreateString("value_old");
    item2 = cJSON_CreateString("value_other");
    cJSON_AddItemToObject(root, "alpha", item1);
    cJSON_AddItemToObject(root, "beta", item2);

    // step 3: Operate / Validate
    got_alpha_before = cJSON_GetObjectItem(root, "alpha");
    equal_before = cJSON_Compare(got_alpha_before, item1, 1);
    replacement = cJSON_CreateString("value_new");
    replaced = cJSON_ReplaceItemViaPointer(root, got_alpha_before, replacement);
    got_alpha_after = cJSON_GetObjectItem(root, "alpha");
    equal_after = cJSON_Compare(got_alpha_after, replacement, 1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}