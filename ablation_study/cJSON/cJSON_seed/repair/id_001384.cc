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
//<ID> 1384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "topnum", 42.0);
    cJSON_AddNumberToObject(root, "small", 7.0);

    // step 2: Configure
    cJSON *top_item = cJSON_GetObjectItem(root, "topnum");
    double top_val = cJSON_GetNumberValue(top_item);
    cJSON *small_item = cJSON_GetObjectItem(root, "small");
    double small_val = cJSON_GetNumberValue(small_item);

    // step 3: Operate and Validate
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    double sum = top_val + small_val;
    buffer[0] = (char)('0' + ((int)top_val % 10));
    buffer[1] = (char)('0' + ((int)small_val % 10));
    buffer[2] = (char)('0' + ((int)sum % 10));
    buffer[3] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}