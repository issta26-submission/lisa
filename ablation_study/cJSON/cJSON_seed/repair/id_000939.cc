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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(root, "name", "fuzzer_node");
    cJSON_AddNumberToObject(root, "version", 3.14);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"nested\":true}");
    cJSON_AddItemToObject(config, "raw", raw);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "tester");

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement, "replaced", 7.0);
    cJSON_bool replaced_flag = cJSON_ReplaceItemViaPointer(config, raw, replacement);
    (void)replaced_flag;
    cJSON *replaced_node = cJSON_GetObjectItemCaseSensitive(config, "raw");
    cJSON *replaced_value = cJSON_GetObjectItemCaseSensitive(replaced_node, "replaced");
    double replaced_number = cJSON_GetNumberValue(replaced_value);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    (void)replaced_number;
    (void)name_str;
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}