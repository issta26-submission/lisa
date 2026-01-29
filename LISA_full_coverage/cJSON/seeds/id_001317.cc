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
//<ID> 1317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_AddObjectToObject(root, "inner");
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "always_true", true_item);
    cJSON_AddFalseToObject(inner, "always_false");

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(inner, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}