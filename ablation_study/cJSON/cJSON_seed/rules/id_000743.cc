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
//<ID> 743
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
    cJSON *cmp_target = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("value1");
    cJSON_AddItemToObject(root, "key", orig);
    got_item = cJSON_GetObjectItem(root, "key");
    replacement = cJSON_CreateString("newvalue");
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);

    // step 3: Operate / Validate
    cmp_target = cJSON_CreateString("newvalue");
    cmp_result = cJSON_Compare((const cJSON *const)replacement, (const cJSON *const)cmp_target, 1);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(cmp_target);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}