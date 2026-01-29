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
//<ID> 2113
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
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *n2 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *threshold = cJSON_AddNumberToObject(root, "threshold", 10.5);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    double detached_val = cJSON_GetNumberValue(detached);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (int)detached_val % 10);
    buffer[1] = printed ? printed[0] : '\0';
    buffer[2] = (char)('0' + (int)(threshold->valuedouble));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}