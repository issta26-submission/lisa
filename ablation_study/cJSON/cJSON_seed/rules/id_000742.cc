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
//<ID> 742
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    cJSON *root = NULL;
    cJSON *orig = NULL;
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    cJSON_bool cmp_before = 0;
    cJSON_bool cmp_after = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "key", orig);

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "key");
    cmp_before = cJSON_Compare(got_item, orig, 1);
    replacement = cJSON_CreateString("new_value");
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    got_item = cJSON_GetObjectItem(root, "key");
    cmp_after = cJSON_Compare(got_item, replacement, 1);
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}