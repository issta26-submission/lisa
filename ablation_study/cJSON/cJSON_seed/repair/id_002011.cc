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
//<ID> 2011
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *item_old = cJSON_CreateString("old_value");
    cJSON *item_ref = cJSON_CreateStringReference("ref_value");
    cJSON_AddItemToArray(array, item_old);
    cJSON_AddItemToArray(array, item_ref);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_ReplaceItemViaPointer(array, item_old, replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(array);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = printed[1];
    buffer[2] = printed[2];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}