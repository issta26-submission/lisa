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
//<ID> 1926
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
    cJSON *t1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, t1);
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, t2);
    cJSON_AddNullToObject(root, "missing");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);

    // step 3: Operate and Validate
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    buffer[0] = (char)('0' + printed_ok);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}