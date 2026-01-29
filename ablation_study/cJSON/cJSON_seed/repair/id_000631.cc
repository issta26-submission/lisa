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
//<ID> 631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "count", count);
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(metrics, "active", active);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(3.0));

    // step 2: Configure
    double before_value = cJSON_GetNumberValue(count);
    double set_result = cJSON_SetNumberHelper(count, 84.5);
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate and Validate
    cJSON *replacement_active = cJSON_CreateBool(0);
    cJSON_ReplaceItemViaPointer(metrics, active, replacement_active);
    double after_value = cJSON_GetNumberValue(count);
    cJSON_AddNumberToObject(root, "count_check", after_value);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}