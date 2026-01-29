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
//<ID> 1374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *orig = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "greeting", orig);
    cJSON_AddNumberToObject(root, "count", 3.14);

    // step 2: Configure
    cJSON *gptr = cJSON_GetObjectItem(root, "greeting");
    cJSON_IsNull(gptr);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, gptr, replacement);

    // step 3: Operate & Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}