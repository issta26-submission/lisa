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
//<ID> 2136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", array);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, null_item);
    cJSON *array_entry = cJSON_CreateString("entry_one");
    cJSON_AddItemToArray(array, array_entry);
    cJSON *label = cJSON_AddStringToObject(root, "label", "initial_label");
    char *setres = cJSON_SetValuestring(label, "updated_label");

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(array, 0);

    // step 4: Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = setres[0];
    const char *astr = cJSON_GetStringValue(array_entry);
    buffer[1] = astr[0];
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}