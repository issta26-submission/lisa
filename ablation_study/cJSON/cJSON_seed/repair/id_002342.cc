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
//<ID> 2342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"inner\":\"value\",\"num\":7}");
    cJSON *s1 = cJSON_CreateString("standalone-string");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "from_parsed", parsed);
    cJSON_AddItemToObjectCS(root, "plain", s1);

    // step 3: Operate
    char *rendered = cJSON_PrintBuffered(root, 256, 1);
    cJSON *inner_item = cJSON_GetObjectItem(parsed, "inner");
    const char *inner_str = cJSON_GetStringValue(inner_item);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (rendered && rendered[0]) ? rendered[0] : '\0';
    buffer[1] = (inner_str && inner_str[0]) ? inner_str[0] : '\0';
    cJSON_free(rendered);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}