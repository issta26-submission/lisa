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
//<ID> 748
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
    const char *key = "greeting";
    cJSON *root = NULL;
    cJSON *orig_str = NULL;
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    cJSON_bool added = 0;
    cJSON_bool are_equal = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig_str = cJSON_CreateString("hello");
    added = cJSON_AddItemToObject(root, key, orig_str);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, key);
    replacement = cJSON_CreateString("Hello");
    are_equal = cJSON_Compare(got_item, replacement, 0);
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}