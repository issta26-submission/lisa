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
//<ID> 1597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *null_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_flag = NULL;
    char *out = NULL;
    double number_value = 0.0;
    cJSON_bool flag_is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "number", num_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);
    flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", flag_item);

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItem(root, "number");
    number_value = cJSON_GetNumberValue(retrieved_num);
    retrieved_flag = cJSON_GetObjectItem(root, "disabled");
    flag_is_false = cJSON_IsFalse(retrieved_flag);
    out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "number");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}