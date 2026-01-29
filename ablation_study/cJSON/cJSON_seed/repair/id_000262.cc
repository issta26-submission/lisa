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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"a\":3.5,\"b\":7}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON *added_c = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "c", added_c);

    // step 3: Operate and Validate
    cJSON *item_a = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(item_a);
    cJSON *replacement_b = cJSON_CreateNumber(a_val + 1.5);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "b", replacement_b);
    (void)replaced;

    // step 4: Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}