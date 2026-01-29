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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(0.0);
    cJSON *str_ref = cJSON_CreateStringReference("example_ref");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "fixed_number", num1);
    double set_res = cJSON_SetNumberHelper(num2, 7.0);
    cJSON_AddItemToObjectCS(root, "set_number", num2);
    cJSON_AddItemToObjectCS(root, "ref", str_ref);

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetObjectItem(root, "set_number");
    double val = cJSON_GetNumberValue(got_num);
    cJSON *derived = cJSON_CreateNumber(val + set_res);
    cJSON_AddItemToObjectCS(root, "derived", derived);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}