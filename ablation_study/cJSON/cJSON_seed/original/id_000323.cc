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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int numbers[] = {10, 20, 30};
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "ints", int_array);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(int_array, 1);
    cJSON *duplicate_item = cJSON_Duplicate(second_item, 0);
    cJSON_bool equal = cJSON_Compare(second_item, duplicate_item, 1);
    cJSON_AddBoolToObject(root, "second_equals_duplicate", equal);
    cJSON_AddItemToObject(root, "duplicate", duplicate_item);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(int_array, 0);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_AddBoolToObject(root, "prealloc_ok", prealloc_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}