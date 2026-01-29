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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"name\":\"fuzzer\",\"flags\":[true,false,true],\"values\":[1.5,2.5]}";
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *flags_arr = NULL;
    cJSON *flag0_item = NULL;
    cJSON *flag2_item = NULL;
    cJSON *values_arr = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    double values_sum = 0.0;
    cJSON_bool is_flag0_true = 0;
    cJSON_bool is_flag2_true = 0;
    char *buf = (char *)cJSON_malloc((size_t)64);
    memset(buf, 0, 64);

    // step 2: Setup / Parse
    root = cJSON_Parse(json_text);
    name_item = cJSON_GetObjectItem(root, "name");
    flags_arr = cJSON_GetObjectItem(root, "flags");
    values_arr = cJSON_GetObjectItem(root, "values");

    // step 3: Operate / Validate
    {
        const char *name_str = cJSON_GetStringValue(name_item);
        memcpy(buf, name_str, strlen(name_str) + 1);
    }
    flag0_item = cJSON_GetArrayItem(flags_arr, 0);
    flag2_item = cJSON_GetArrayItem(flags_arr, 2);
    is_flag0_true = cJSON_IsTrue(flag0_item);
    is_flag2_true = cJSON_IsTrue(flag2_item);
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values_arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values_arr, 1));
    values_sum = v0 + v1;
    buf[0] = (char)('0' + (is_flag0_true ? 1 : 0));
    buf[1] = (char)('0' + (is_flag2_true ? 1 : 0));
    buf[2] = (char)('0' + ((int)values_sum % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)is_flag0_true;
    (void)is_flag2_true;
    (void)v0;
    (void)v1;
    (void)values_sum;
    return 66;
}