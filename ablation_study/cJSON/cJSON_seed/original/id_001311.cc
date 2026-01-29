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
//<ID> 1311
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", true_item);
    cJSON *false_item = cJSON_AddFalseToObject(root, "disabled");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *version = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(meta, "version", version);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);

    // step 4: Validate & Cleanup
    (void)equal;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}