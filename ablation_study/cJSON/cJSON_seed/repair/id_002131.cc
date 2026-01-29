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
//<ID> 2131
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

    // step 2: Configure
    cJSON *nil_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, nil_item);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello");
    char *updated = cJSON_SetValuestring(greeting, "world");

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(array, 0);
    cJSON *tmp = cJSON_CreateString("array_entry");
    cJSON_AddItemToArray(array, tmp);
    cJSON *fetched = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    const char *gstr = cJSON_GetStringValue(fetched);
    int arr_size = cJSON_GetArraySize(array);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = gstr ? gstr[0] : '\0';
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}