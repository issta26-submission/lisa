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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *flag = cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, it1);
    cJSON *it2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, it2);
    cJSON_AddStringToObject(root, "name", "nodeX");

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItemCaseSensitive(root, "items");
    int count = cJSON_GetArraySize(got_items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)ok;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}