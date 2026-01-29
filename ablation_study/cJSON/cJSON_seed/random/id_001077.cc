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
//<ID> 1077
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
    double num_value = 0.0;
    const char *str_value = (const char *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("hello");

    // step 3: Configure - attach items to the root object
    add_ok1 = cJSON_AddItemToObject(root, "answer", num_item);
    add_ok2 = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate - read values from created items
    num_value = cJSON_GetNumberValue(num_item);
    str_value = cJSON_GetStringValue(str_item);

    // step 5: Validate - compute a small validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(add_ok1) + (int)(add_ok2) + (int)(num_value == 42.0) + (int)(str_value && str_value[0] == 'h');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}