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
//<ID> 741
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
    cJSON *original = NULL;
    cJSON *got = NULL;
    cJSON *replacement = NULL;
    cJSON *got_after = NULL;
    cJSON_bool added = 0;
    cJSON_bool cmp_before = 0;
    cJSON_bool replaced = 0;
    cJSON_bool cmp_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    original = cJSON_CreateString("original_value");
    added = cJSON_AddItemToObject(root, "key", original);
    replacement = cJSON_CreateString("replacement_value");
    cmp_before = cJSON_Compare(original, replacement, 1);

    // step 3: Operate / Validate
    got = cJSON_GetObjectItem(root, "key");
    replaced = cJSON_ReplaceItemViaPointer(root, got, replacement);
    got_after = cJSON_GetObjectItem(root, "key");
    cmp_after = cJSON_Compare(got_after, replacement, 1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}