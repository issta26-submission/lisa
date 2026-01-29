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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_str[] = "{\"a\":1,\"b\":2,\"nested\":{\"x\":3}}";
    size_t json_len = sizeof(json_str) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_str, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "b", replacement);
    cJSON *new_field = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "new_field", new_field);

    // step 3: Operate
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON *nested = cJSON_GetObjectItem(root, "nested");
    cJSON *x_item = cJSON_GetObjectItem(nested, "x");
    double x_val = cJSON_GetNumberValue(x_item);
    double sum = a_val + x_val;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parse_end;
    return 66;
}