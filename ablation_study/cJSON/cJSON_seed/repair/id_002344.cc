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
//<ID> 2344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_input = "{\"initial\":\"orig\"}";
    cJSON *root = cJSON_Parse(json_input);

    // step 2: Configure
    cJSON *new_string = cJSON_CreateString("appended");
    cJSON_AddItemToObjectCS(root, "extra", new_string);

    // step 3: Operate
    cJSON *initial_item = cJSON_GetObjectItem(root, "initial");
    const char *initial_val = cJSON_GetStringValue(initial_item);
    cJSON *extra_item = cJSON_GetObjectItem(root, "extra");
    const char *extra_val = cJSON_GetStringValue(extra_item);
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (initial_val && initial_val[0]) ? initial_val[0] : '\0';
    buffer[2] = (extra_val && extra_val[0]) ? extra_val[0] : '\0';
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}