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
//<ID> 1203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":1,\"b\":2,\"nested\":{\"x\":5}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "from_code", 3.1415);
    cJSON *b_item = cJSON_GetObjectItem(parsed, "b");
    double b_value = cJSON_GetNumberValue(b_item);
    cJSON_AddNumberToObject(root, "copied_b", b_value);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "a");
    cJSON_bool detached_invalid = cJSON_IsInvalid(detached);
    cJSON_AddNumberToObject(root, "detached_invalid", (double)detached_invalid);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(detached);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}