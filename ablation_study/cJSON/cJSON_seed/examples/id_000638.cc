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
//<ID> 638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *meta = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(data, num1);
    cJSON_AddItemToArray(data, str1);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "creator", "tester");

    // step 2: Configure
    cJSON *dup_data = cJSON_Duplicate(data, 1);
    cJSON_AddItemToObject(root, "dup", dup_data);
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToArray(dup_data, extra);
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    cJSON *parsed_dup = cJSON_GetObjectItem(parsed, "dup");
    cJSON_DeleteItemFromObject(parsed, "meta");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}