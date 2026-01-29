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
//<ID> 921
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, num1);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, num2);
    cJSON *label = cJSON_CreateString("nodeA");
    cJSON_AddItemToObject(meta, "label", label);

    // step 3: Operate and Validate
    cJSON *active_item = cJSON_GetObjectItemCaseSensitive(root, "active");
    cJSON_bool is_active = cJSON_IsTrue(active_item);
    cJSON_AddNumberToObject(root, "active_flag", (double)is_active);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}