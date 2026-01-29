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
//<ID> 873
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
    cJSON_AddItemToObjectCS(root, "meta", meta);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateStringReference(ver);
    cJSON_AddItemToObjectCS(meta, "version", ver_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "maybe_null", null_item);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(root, "answer", num_item);

    // step 2: Configure
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)pre_ok;
    const char *version_value = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "version"));
    cJSON *version_ref = cJSON_CreateStringReference(version_value);
    cJSON_AddItemToObjectCS(root, "version_ref", version_ref);

    // step 3: Operate
    char *flat = cJSON_PrintUnformatted(root);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    (void)equal;
    char *pretty_copy = cJSON_Print(copy);

    // step 4: Validate & Cleanup
    cJSON_free(flat);
    cJSON_free(pretty_copy);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}