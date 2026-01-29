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
//<ID> 2111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddNumberToObject(root, "count", 7.0);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, s1);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    double val = cJSON_GetNumberValue(detached);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (int)val);
    cJSON *remaining_first = cJSON_GetArrayItem(arr, 0);
    const char *first_char = cJSON_GetStringValue(remaining_first);
    buffer[1] = first_char ? first_char[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}