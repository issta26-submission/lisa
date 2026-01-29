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
//<ID> 1040
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"init\":true,\"values\":[1,2]}";
    cJSON *root1 = cJSON_Parse(json_text);
    const char *parse_end = NULL;
    cJSON *root2 = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), &parse_end, 1);
    cJSON *constructed_array = cJSON_CreateArray();
    cJSON *num_pi = cJSON_CreateNumber(3.1415);
    cJSON *num_answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(constructed_array, num_pi);
    cJSON_AddItemToArray(constructed_array, num_answer);

    // step 2: Configure
    cJSON *str_ref = cJSON_CreateStringReference("ref_label");
    cJSON_AddItemToObject(root1, "constructed", constructed_array);
    cJSON_AddItemToObject(root1, "ref", str_ref);

    // step 3: Operate & Validate
    cJSON *values = cJSON_GetObjectItem(root1, "values");
    int orig_size = cJSON_GetArraySize(values);
    cJSON *size_num = cJSON_CreateNumber((double)orig_size + 1.0);
    cJSON_AddItemToObject(root1, "orig_plus_one", size_num);
    cJSON *parsed_values = cJSON_GetObjectItem(root2, "values");
    cJSON *first = cJSON_GetArrayItem(parsed_values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *first_copy = cJSON_CreateNumber(first_val);
    cJSON_AddItemToArray(constructed_array, first_copy);

    // step 4: Cleanup
    cJSON_Delete(root2);
    cJSON_Delete(root1);
    return 66; // API sequence test completed successfully
}