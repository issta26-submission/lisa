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
//<ID> 1848
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(40.0));
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    /* copy up to buffer capacity - 1 to ensure null termination */
    strncpy(buffer, printed, 255);
    cJSON *got_detached = cJSON_GetObjectItem(root, "detached");
    double detached_val = cJSON_GetNumberValue(got_detached);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (((int)detached_val) % 10));
    buffer[2] = (char)('0' + (cJSON_GetArraySize(items) % 10));
    (void)detached_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}