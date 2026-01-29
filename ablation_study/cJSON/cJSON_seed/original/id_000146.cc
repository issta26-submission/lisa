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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *n = cJSON_CreateNumber(3.1415);
    cJSON *s = cJSON_CreateString("hello");
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(obj, "n", n);
    cJSON_AddItemToObjectCS(obj, "s", s);
    cJSON_AddItemToObjectCS(obj, "x", nul);
    cJSON_AddItemToObjectCS(root, "data", obj);

    // step 2: Configure
    cJSON *data_ptr = cJSON_GetObjectItemCaseSensitive(root, "data");
    cJSON *n_ptr = cJSON_GetObjectItemCaseSensitive(data_ptr, "n");
    cJSON *s_ptr = cJSON_GetObjectItemCaseSensitive(data_ptr, "s");
    cJSON *x_ptr = cJSON_GetObjectItemCaseSensitive(data_ptr, "x");
    cJSON_bool is_null = cJSON_IsNull(x_ptr);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    double n_val = cJSON_GetNumberValue(n_ptr);
    const char *s_val = cJSON_GetStringValue(s_ptr);
    int summary = (int)n_val + (int)s_val[0] + (int)is_null + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}