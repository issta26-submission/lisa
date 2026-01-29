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
//<ID> 1421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));

    // step 2: Configure
    cJSON *fetched_config = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_item = cJSON_GetObjectItem(fetched_config, "enabled");
    double enabled_flag = (double)cJSON_IsTrue(enabled_item);
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "items_count", (double)count);
    cJSON_AddNumberToObject(root, "enabled_flag", enabled_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t len = printed ? (size_t)strlen(printed) : 0;
    char *scratch = (char *)cJSON_malloc(len + 8);
    memset(scratch, 0, (size_t)(len + 8));
    scratch[0] = printed[0];
    scratch[1] = (char)('0' + (count % 10));
    scratch[2] = (char)('0' + ((int)enabled_flag % 10));
    cJSON_free(scratch);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}