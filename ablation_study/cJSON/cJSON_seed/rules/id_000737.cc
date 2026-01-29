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
//<ID> 737
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
    cJSON *flags = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    int array_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    flags = cJSON_AddArrayToObject(root, "flags");
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, true_item);
    cJSON_AddItemToArray(flags, false_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "flags");
    array_size = cJSON_GetArraySize(detached);
    printed = cJSON_Print(detached);
    scratch[0] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}