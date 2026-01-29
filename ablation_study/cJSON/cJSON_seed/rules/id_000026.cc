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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_a = cJSON_CreateNumber(10.0);
    cJSON *num_b = cJSON_CreateNumber(20.5);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON *derived_num = NULL;
    char *printed = NULL;
    double num_b_val = 0.0;
    int flag = 0;

    // step 2: Configure (attach initial items)
    cJSON_AddItemToObject(root, "alpha", num_a);
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate / Validate (replace, compute, attach)
    cJSON_ReplaceItemViaPointer(root, num_a, num_b);
    num_b_val = cJSON_GetNumberValue(num_b);
    flag = (int)cJSON_IsTrue(flag_item);
    derived_num = cJSON_CreateNumber(num_b_val + (double)flag);
    cJSON_AddItemToObject(root, "derived", derived_num);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}