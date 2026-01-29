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
//<ID> 749
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    cJSON *root = NULL;
    cJSON *orig_str = NULL;
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    cJSON_bool cmp_before = 0;
    cJSON_bool replaced = 0;
    cJSON_bool cmp_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig_str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", orig_str);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "greeting");
    replacement = cJSON_CreateString("world");
    cmp_before = cJSON_Compare(got_item, replacement, 1);
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    cmp_after = cJSON_Compare(cJSON_GetObjectItem(root, "greeting"), replacement, 1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}