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
//<ID> 924
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddStringToObject(meta, "name", "example");

    // step 3: Operate and Validate
    cJSON *foundActive = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON_bool active_flag = cJSON_IsTrue(foundActive);
    cJSON_AddNumberToObject(root, "active_flag", (double)active_flag);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)ok;
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *snapshot = cJSON_PrintUnformatted(dup);
    cJSON_free(snapshot);
    cJSON_Delete(dup);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}