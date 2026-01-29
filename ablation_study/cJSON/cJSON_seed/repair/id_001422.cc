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
//<ID> 1422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));
    cJSON_AddTrueToObject(data, "enabled");

    // step 2: Configure
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(data, "count", (double)count);
    cJSON *enabled_item = cJSON_GetObjectItem(data, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);

    // step 3: Operate and Validate
    char *rendered = cJSON_PrintUnformatted(root);
    size_t len = rendered ? (size_t)strlen(rendered) : 0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, (size_t)(len + 16));
    scratch[0] = rendered ? rendered[0] : '0';
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + ((count / 10) % 10));
    scratch[3] = enabled_flag ? '1' : '0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(rendered);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}