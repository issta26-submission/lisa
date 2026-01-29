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
//<ID> 2343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *input = "{\"initial\": 123}";
    cJSON *root = cJSON_Parse(input);

    // step 2: Configure
    cJSON *added = cJSON_CreateString("added-value");
    cJSON_AddItemToObjectCS(root, "new_key", added);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (printed && printed[0]) ? printed[0] : '\0';
    buffer[1] = (char)(root->type & 0xFF);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}