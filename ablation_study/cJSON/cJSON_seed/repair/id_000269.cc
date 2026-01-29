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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"a\":1,\"b\":2}";
    const char *parse_end = NULL;
    size_t json_len = sizeof("{\"a\":1,\"b\":2}") - 1;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *replacement_a = cJSON_CreateNumber(3.14);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "a", replacement_a);
    cJSON *new_c = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "c", new_c);

    // step 3: Operate and Validate
    cJSON *b_item = cJSON_GetObjectItemCaseSensitive(root, "b");
    double b_val = cJSON_GetNumberValue(b_item);
    cJSON_AddNumberToObject(root, "b_copy", b_val);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}