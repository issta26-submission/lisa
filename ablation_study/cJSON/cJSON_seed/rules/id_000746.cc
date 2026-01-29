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
//<ID> 746
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
    cJSON *root = NULL;
    cJSON *orig = NULL;
    cJSON *temp_ref = NULL;
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    cJSON *after_item = NULL;
    cJSON_bool equal_before = 0;
    cJSON_bool replaced = 0;
    cJSON_bool equal_after = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "key", orig);
    temp_ref = cJSON_CreateString("original");

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "key");
    equal_before = cJSON_Compare(got_item, temp_ref, 1);
    replacement = cJSON_CreateString("replaced");
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    after_item = cJSON_GetObjectItem(root, "key");
    equal_after = cJSON_Compare(after_item, replacement, 1);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(temp_ref);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}