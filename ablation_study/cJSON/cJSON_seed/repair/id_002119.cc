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
//<ID> 2119
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
    cJSON_AddItemToObject(root, "numbers", array);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON *added = cJSON_AddNumberToObject(root, "initial_sum", 30.0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(array, 1);
    double val_detached = cJSON_GetNumberValue(detached);
    double val_added = cJSON_GetNumberValue(added);
    cJSON_AddNumberToObject(root, "detached_value", val_detached);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('0' + (int)val_detached % 10);
    buffer[1] = (char)('0' + (int)val_added % 10);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}