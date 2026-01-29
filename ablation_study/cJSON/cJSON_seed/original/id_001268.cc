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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(values, 1, replacement);
    (void)replaced;

    // step 3: Operate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON *item1 = cJSON_GetArrayItem(got_values, 1);
    cJSON_bool is_num = cJSON_IsNumber(item1);
    (void)is_num;
    char *printed = cJSON_PrintUnformatted(root);
    int printed_len = (int)strlen(printed);
    char *buf = (char *)cJSON_malloc((size_t)printed_len + 1);
    memset(buf, 0, (size_t)printed_len + 1);
    memcpy(buf, printed, (size_t)printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    double value = cJSON_GetNumberValue(item1);
    (void)value;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}