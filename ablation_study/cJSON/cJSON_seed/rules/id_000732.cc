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
//<ID> 732
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
    cJSON *flags_array = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached_mark = NULL;
    char *printed = NULL;
    int flags_count = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    flags_array = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags_array, true_item);
    cJSON_AddItemToArray(flags_array, false_item);
    cJSON_AddItemToObject(root, "flags", flags_array);
    cJSON_AddTrueToObject(root, "mark");
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate / Validate
    flags_count = cJSON_GetArraySize(flags_array);
    memcpy(scratch, &flags_count, sizeof(flags_count));
    detached_mark = cJSON_DetachItemFromObjectCaseSensitive(root, "mark");
    printed = cJSON_Print(root);
    memcpy(scratch + 16, printed, 64);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_mark);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}