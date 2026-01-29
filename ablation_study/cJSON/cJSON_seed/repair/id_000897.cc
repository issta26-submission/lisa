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
//<ID> 897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *true1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true1);
    cJSON *true2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true2);

    // step 2: Configure
    int buffer_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buffer_len);
    memset(buffer, 0, (size_t)buffer_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buffer_len, 1);
    cJSON_AddNumberToObject(root, "printed_flag", (double)printed);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)size);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON_AddNumberToObject(root, "first_is_true", (double)first_is_true);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(dup, root, 1);
    cJSON_AddNumberToObject(dup, "equal_to_root", (double)equal);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}