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
//<ID> 730
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
    cJSON *ref_str = cJSON_CreateStringReference("ref_hello");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *plain_str = cJSON_CreateString("direct_hello");

    // step 2: Configure
    cJSON_AddItemToArray(data, ref_str);
    cJSON_AddItemToArray(data, num);
    cJSON_AddItemToArray(data, plain_str);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("meta_value"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = snapshot ? strlen(snapshot) : 0;
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    if (snapshot) memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    cJSON *first_item = cJSON_GetArrayItem(parsed_data, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    (void)version;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}