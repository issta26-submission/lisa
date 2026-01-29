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
//<ID> 2314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));
    cJSON_AddItemToObject(root, "owner", cJSON_CreateString("tester"));

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(values, cJSON_CreateString("hello"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "type", cJSON_CreateString("sample"));
    cJSON_AddItemToObject(meta, "version", cJSON_CreateNumber(1.0));

    // step 3: Operate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    int count = cJSON_GetArraySize(retrieved_values);
    cJSON *first_item = cJSON_GetArrayItem(retrieved_values, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (char)('0' + (count >= 0 && count < 10 ? count : 9));
    (void)first_str;
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}