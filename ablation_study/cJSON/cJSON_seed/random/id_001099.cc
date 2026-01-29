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
//<ID> 1099
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    cJSON *fetched_false = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_str = (const char *)0;
    double retrieved_num = 0.0;
    cJSON_bool is_retrieved_str = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("pi_approx");

    // step 3: Configure - attach number, string and a false boolean to the root
    cJSON_AddItemToObject(root, "pi", num_item);
    cJSON_AddItemToObject(root, "name", str_item);
    false_item = cJSON_AddFalseToObject(root, "valid");

    // step 4: Operate - fetch the items and inspect types/values
    fetched_num = cJSON_GetObjectItem(root, "pi");
    fetched_str = cJSON_GetObjectItem(root, "name");
    fetched_false = cJSON_GetObjectItem(root, "valid");
    is_retrieved_str = cJSON_IsString(fetched_str);
    retrieved_str = cJSON_GetStringValue(fetched_str);
    retrieved_num = cJSON_GetNumberValue(fetched_num);

    // step 5: Validate - serialize, free the buffer and compute a small score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(fetched_num != (cJSON *)0) + (int)(fetched_str != (cJSON *)0) + (int)(fetched_false != (cJSON *)0) + (int)(is_retrieved_str) + (int)(retrieved_num > 0.0) + (int)(retrieved_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}