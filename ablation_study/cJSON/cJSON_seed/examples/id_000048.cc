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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\": [10.5, 20.25], \"meta\": \"parsed_source\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    double local_vals[3] = {1.0, 2.5, 3.75};
    cJSON *double_arr = cJSON_CreateDoubleArray(local_vals, 3);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool added_numbers = cJSON_AddItemToObjectCS(root, "numbers", double_arr);
    cJSON_bool added_flag = cJSON_AddItemToObjectCS(root, "flag", false_item);
    (void)added_numbers;
    (void)added_flag;

    // step 3: Operate & Validate
    cJSON_bool has_numbers_root = cJSON_HasObjectItem(root, "numbers");
    cJSON_bool has_numbers_parsed = cJSON_HasObjectItem(parsed, "numbers");
    double presence_sum = (double)(has_numbers_root + has_numbers_parsed);
    cJSON *presence_num = cJSON_CreateNumber(presence_sum);
    cJSON_bool added_presence = cJSON_AddItemToObjectCS(root, "presence_sum", presence_num);
    (void)added_presence;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}