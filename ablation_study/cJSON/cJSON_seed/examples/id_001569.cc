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
//<ID> 1569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *init_num = cJSON_AddNumberToObject(root, "initial", 42.0);
    cJSON *flag_item_initial = cJSON_AddTrueToObject(root, "flag");

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(8.5);
    cJSON_AddItemToObject(root, "added_num", extra_num);

    // step 3: Operate & Validate
    cJSON *initial_item = cJSON_GetObjectItem(root, "initial");
    cJSON *added_item = cJSON_GetObjectItem(root, "added_num");
    double initial_val = cJSON_GetNumberValue(initial_item);
    double added_val = cJSON_GetNumberValue(added_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    double flag_val = (double)cJSON_IsTrue(flag_item);
    double total = initial_val + added_val + flag_val;
    cJSON *sum_node = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_node);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}