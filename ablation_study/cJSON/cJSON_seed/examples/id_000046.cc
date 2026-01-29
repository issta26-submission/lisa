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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"x\": 10.5, \"y\": [1,2,3]}";
    cJSON *parsed = cJSON_Parse(json_text);
    double nums[4] = {1.1, 2.2, 3.3, 4.4};
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 4);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_bool added1 = cJSON_AddItemToObjectCS(root, "doubles", dbl_array);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool added2 = cJSON_AddItemToObjectCS(root, "flag", false_item);
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_bool added3 = cJSON_AddItemToObjectCS(root, "parsed_copy", dup_parsed);
    cJSON_Delete(parsed);

    // step 3: Operate & Validate
    cJSON_bool has_flag = cJSON_HasObjectItem(root, "flag");
    cJSON_bool has_parsed = cJSON_HasObjectItem(root, "parsed_copy");
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    (void)added1; (void)added2; (void)added3; (void)has_flag; (void)has_parsed;
    return 66;
}