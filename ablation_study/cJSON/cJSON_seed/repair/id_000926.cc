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
//<ID> 926
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
    cJSON *flag_item = cJSON_AddTrueToObject(root, "flag");

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("entry"));
    cJSON_AddNumberToObject(root, "id", 7.0);

    // step 3: Operate and Validate
    cJSON *fetched_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(fetched_flag);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed_ok;
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)flag_is_true;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}