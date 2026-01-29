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
//<ID> 1200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"a\":1,\"b\":2,\"nested\":{\"x\":10}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added", 42.5);
    cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObject(parsed, "nested");
    cJSON_bool is_invalid = cJSON_IsInvalid(detached);
    cJSON_AddNumberToObject(root, "nested_invalid", (double)is_invalid);
    char *printed_root = cJSON_Print(root);
    char *printed_detached = cJSON_Print(detached);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}