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
//<ID> 575
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(data, "pi", num);
    cJSON *text = cJSON_CreateString("initial");
    cJSON_AddItemToObject(data, "msg", text);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(data, "enabled", flag);

    // step 2: Configure
    cJSON_SetValuestring(text, "updated");
    cJSON *double_num = cJSON_CreateNumber(cJSON_GetNumberValue(num) * 2.0);
    cJSON_AddItemToObject(data, "pi_double", double_num);

    // step 3: Operate and Validate
    char *got_str = cJSON_GetStringValue(text);
    double got_num = cJSON_GetNumberValue(num);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    double sum = got_num + cJSON_GetNumberValue(double_num) + (double)(is_false) * 0.0;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}