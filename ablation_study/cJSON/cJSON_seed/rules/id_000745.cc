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
//<ID> 745
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
    char *printed = NULL;
    cJSON *root = NULL;
    cJSON *other = NULL;
    cJSON *orig = NULL;
    cJSON *replacement = NULL;
    cJSON *got_item = NULL;
    cJSON *other_item = NULL;
    cJSON_bool added_root = 0;
    cJSON_bool added_other = 0;
    cJSON_bool replaced = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("old_value");
    added_root = cJSON_AddItemToObject(root, "key", orig);
    replacement = cJSON_CreateString("new_value");
    other = cJSON_CreateObject();
    added_other = cJSON_AddItemToObject(other, "key", cJSON_CreateString("new_value"));

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "key");
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    got_item = cJSON_GetObjectItem(root, "key");
    other_item = cJSON_GetObjectItem(other, "key");
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    equal = cJSON_Compare(got_item, other_item, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(other);

    // API sequence test completed successfully
    return 66;
}