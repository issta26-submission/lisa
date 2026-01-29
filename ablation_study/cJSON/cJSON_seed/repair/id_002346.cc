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
//<ID> 2346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *input = "{\"greeting\":\"hello\",\"count\":3}";
    cJSON *root = cJSON_Parse(input);
    cJSON *note = cJSON_CreateString("note-added");
    cJSON_AddItemToObjectCS(root, "note", note);

    // step 2: Configure
    cJSON *greeting = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(greeting);
    cJSON *extra = cJSON_CreateString("extra-info");
    cJSON_AddItemToObjectCS(root, "extra", extra);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (greet_val && greet_val[0]) ? greet_val[0] : '\0';
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}