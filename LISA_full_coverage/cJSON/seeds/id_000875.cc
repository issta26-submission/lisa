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
//<ID> 875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version_literal = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *ver_str = cJSON_CreateString(version_literal);
    cJSON_AddItemToObjectCS(meta, "version", ver_str);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(meta, "nullable", null_item);

    // step 2: Configure
    const char *version_value = cJSON_GetStringValue(ver_str);
    cJSON *ver_copy = cJSON_CreateString(version_value);
    cJSON_AddItemToObjectCS(root, "version_copy", ver_copy);

    // step 3: Operate
    char outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, outbuf, (int)sizeof(outbuf), 1);
    (void)pre_ok;
    char *unformatted = cJSON_PrintUnformatted(root);
    (void)outbuf;
    cJSON_free(unformatted);

    // step 4: Validate & Cleanup
    cJSON_bool is_ver_string = cJSON_IsString(cJSON_GetObjectItem(meta, "version"));
    (void)is_ver_string;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}