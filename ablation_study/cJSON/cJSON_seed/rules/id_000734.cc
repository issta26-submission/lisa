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
//<ID> 734
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
    cJSON *settings = NULL;
    cJSON *flags = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached = NULL;
    cJSON *det_flags = NULL;
    char *printed = NULL;
    int flags_count = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    settings = cJSON_AddObjectToObject(root, "settings");
    flags = cJSON_AddArrayToObject(settings, "flags");
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, true_item);
    cJSON_AddItemToArray(flags, false_item);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "settings");
    det_flags = cJSON_GetObjectItemCaseSensitive(detached, "flags");
    flags_count = cJSON_GetArraySize(det_flags);
    printed = cJSON_Print(detached);
    memcpy(scratch, &flags_count, sizeof(flags_count));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}