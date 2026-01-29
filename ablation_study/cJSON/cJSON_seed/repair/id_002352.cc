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
//<ID> 2352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.25);
    cJSON_bool added1 = cJSON_AddItemToArray(array, n1);
    cJSON_bool added2 = cJSON_AddItemToArray(array, n2);
    cJSON_bool added3 = cJSON_AddItemToArray(array, n3);

    // step 2: Configure
    cJSON *n4 = cJSON_CreateNumber(4.75);
    cJSON_bool added4 = cJSON_AddItemToArray(array, n4);

    // step 3: Operate
    cJSON *fetched = cJSON_GetArrayItem(array, 1);
    double fetched_value = cJSON_GetNumberValue(fetched);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (fetched_value != 0.0) ? (char)(int)fetched_value : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(buffer);
    cJSON_Delete(array);
    return 66;
    // API sequence test completed successfully
}