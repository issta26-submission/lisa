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
//<ID> 1539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name_str = cJSON_AddStringToObject(root, "name", "fuzz_tester");
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *pi_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(meta, "pi", pi_num);

    // step 2: Configure
    cJSON *answer_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer_num);
    cJSON *flag_bool = cJSON_CreateBool(0);
    cJSON_AddItemToObject(meta, "flag", flag_bool);
    cJSON_AddStringToObject(meta, "status", "configured");

    // step 3: Operate and Validate
    double pi_val = cJSON_GetNumberValue(pi_num);
    double ans_val = cJSON_GetNumberValue(answer_num);
    double sum = pi_val + ans_val;
    cJSON *sum_num = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_num);
    cJSON_AddStringToObject(meta, "note", "computed_sum_added");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}