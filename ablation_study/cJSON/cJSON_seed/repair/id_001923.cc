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
//<ID> 1923
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, true_item);
    cJSON_AddNullToObject(root, "optional");
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(items, str_item);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = buffer[0];
    buffer[2] = buffer[1];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}