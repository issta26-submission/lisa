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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double nums[3] = {1.5, 2.5, 3.5};
    cJSON *doubles = cJSON_CreateDoubleArray(nums, 3);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *parsed = cJSON_Parse("{\"parsed_key\":123, \"nested\":{\"inner\":false}}");

    // step 2: Configure
    cJSON_bool added_doubles = cJSON_AddItemToObjectCS(root, "doubles", doubles);
    cJSON_bool added_flag = cJSON_AddItemToObjectCS(root, "flag", flag_false);
    cJSON_bool added_parsed = cJSON_AddItemToObjectCS(root, "external", parsed);

    // step 3: Operate & Validate
    cJSON_bool has_doubles = cJSON_HasObjectItem(root, "doubles");
    cJSON_bool has_flag = cJSON_HasObjectItem(root, "flag");
    char *printed = cJSON_PrintUnformatted(root);
    (void)added_doubles;
    (void)added_flag;
    (void)added_parsed;
    (void)has_doubles;
    (void)has_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}