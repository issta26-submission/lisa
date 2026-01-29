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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON *replacement = cJSON_CreateFalse();
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool is_false_flag = 0;
    double flag_value = 0.0;

    // step 2: Setup - populate inner with a named value
    cJSON_AddStringToObject(inner, "key", "value");

    // step 3: Configure - attach inner to root under the name "inner"
    cJSON_AddItemToObject(root, "inner", inner);

    // step 4: Operate - retrieve the pointer to the attached item and replace it with a false value
    retrieved = cJSON_GetObjectItem(root, "inner");
    cJSON_ReplaceItemViaPointer(root, retrieved, replacement);

    // step 5: Validate - verify the replaced item is false and record the result in the root
    is_false_flag = cJSON_IsFalse(cJSON_GetObjectItem(root, "inner"));
    flag_value = (double)is_false_flag;
    cJSON_AddNumberToObject(root, "inner_is_false", flag_value);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}