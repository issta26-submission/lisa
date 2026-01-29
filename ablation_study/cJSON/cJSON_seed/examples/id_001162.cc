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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"count\":3,\"values\":[4,5]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);

    // step 2: Configure
    cJSON_bool added1 = cJSON_AddItemToArray(created_arr, n1);
    cJSON_bool added2 = cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON *parsed_count_item = cJSON_GetObjectItem(parsed, "count");
    double parsed_count = cJSON_GetNumberValue(parsed_count_item);
    cJSON_AddNumberToObject(root, "parsed_count_copy", parsed_count);

    // step 3: Operate & Validate
    cJSON_bool has_flag = cJSON_HasObjectItem(parsed, "flag");
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON_AddNumberToObject(root, "has_flag", (double)has_flag);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_is_false);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}