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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("unparsed:payload");
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *sum_item = NULL;
    cJSON *dup = NULL;
    cJSON_bool raw_flag = 0;
    cJSON_bool equal_flag = 0;
    double a_val = 0.0;
    double b_val = 0.0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "child", child);
    num_a = cJSON_AddNumberToObject(child, "a", 3.14);
    num_b = cJSON_AddNumberToObject(child, "b", 2.72);
    cJSON_AddItemToObject(child, "raw_entry", raw);

    // step 3: Operate / Validate
    a_val = cJSON_GetNumberValue(num_a);
    b_val = cJSON_GetNumberValue(num_b);
    sum_item = cJSON_AddNumberToObject(root, "sum", a_val + b_val);
    raw_flag = cJSON_IsRaw(cJSON_GetObjectItem(child, "raw_entry"));
    dup = cJSON_Duplicate(root, 1);
    equal_flag = cJSON_Compare(root, dup, 1);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}