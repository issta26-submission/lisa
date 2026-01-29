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
//<ID> 1062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version_const = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(version_const);
    cJSON_AddItemToObjectCS(root, "version", ver_item);
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObjectCS(config, "name", name);
    cJSON *threshold = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(config, "threshold", threshold);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *author = cJSON_CreateString("fuzzer");
    cJSON_AddItemToObjectCS(meta, "author", author);

    // step 3: Operate
    char *version_value = cJSON_GetStringValue(ver_item);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(info, "version_copy", cJSON_CreateString(version_value));
    cJSON_AddItemToObjectCS(root, "info", info);
    char *pretty = cJSON_Print(root);
    int prealloc_len = 256;
    char *prealloc_buf = (char *)cJSON_malloc((size_t)prealloc_len);
    memset(prealloc_buf, 0, (size_t)prealloc_len);
    cJSON_PrintPreallocated(root, prealloc_buf, prealloc_len, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}