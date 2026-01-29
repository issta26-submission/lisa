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
//<ID> 2135
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
    cJSON_AddItemToObject(root, "items", array);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, null_item);
    cJSON *label = cJSON_AddStringToObject(root, "label", "original");

    // step 2: Configure
    char *updated_value = cJSON_SetValuestring(label, "updated");
    cJSON *elem = cJSON_CreateString("entry_one");
    cJSON_AddItemToArray(array, elem);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(array, 0);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = updated_value ? updated_value[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}