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
//<ID> 2115
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

    // step 2: Configure
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 0.0);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(items, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver = cJSON_AddNumberToObject(meta, "version", 1.234);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(items, 0);
    double detached_val = cJSON_GetNumberValue(detached);
    double updated_count = cJSON_SetNumberHelper(count_item, detached_val + 1.0);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)updated_count);
    buffer[1] = (char)('0' + (int)printed_ok);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}