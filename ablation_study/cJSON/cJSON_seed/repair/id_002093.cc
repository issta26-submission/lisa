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
//<ID> 2093
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
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_InsertItemInArray(array, 0, str_item);
    cJSON_AddStringToObject(root, "greeting", "world");

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got = cJSON_GetObjectItem(root, "greeting");
    const char *gval = cJSON_GetStringValue(got);
    buffer[1] = gval[0];
    buffer[2] = (char)('0' + cJSON_GetArraySize(array));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}