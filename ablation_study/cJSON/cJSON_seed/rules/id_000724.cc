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
//<ID> 724
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
    cJSON *root = cJSON_CreateObject();
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *true_item1 = cJSON_CreateTrue();
    cJSON *true_item2 = cJSON_CreateTrue();
    cJSON *true_item3 = cJSON_CreateTrue();
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON_bool ok1 = 0;
    cJSON_bool ok2 = 0;

    // step 2: Setup / Configure
    ok1 = cJSON_AddItemToObject(root, "child", child_obj);
    ok1 = cJSON_AddItemToObject(child_obj, "flag", true_item1);
    ok1 = cJSON_ReplaceItemInObjectCaseSensitive(root, "child", true_item2);

    // step 3: Operate / Validate
    ok2 = cJSON_ReplaceItemViaPointer(root, true_item2, true_item3);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}