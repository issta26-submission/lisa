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
//<ID> 2341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("Hello, cJSON!");
    cJSON_AddItemToObjectCS(root, "greeting", greet);
    const char *raw_json = "{\"parsed\":{\"inner\":\"value\"}}";
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON_AddItemToObjectCS(root, "parsed_block", parsed);
    cJSON *tag = cJSON_CreateString("buffered-string");
    cJSON_AddItemToObjectCS(root, "tag", tag);

    // step 2: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Validate
    cJSON *got = cJSON_GetObjectItem(root, "greeting");
    const char *gval = cJSON_GetStringValue(got);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (gval && gval[0]) ? gval[0] : '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}