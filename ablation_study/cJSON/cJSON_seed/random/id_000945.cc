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
//<ID> 945
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_num = "pi";
    const char *key_name = "name";
    const char *name_val = "example_name";
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *retrieved_str = (char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create numeric and string items and attach to root
    num_item = cJSON_CreateNumber(3.14);
    str_item = cJSON_CreateString(name_val);
    add_ok1 = cJSON_AddItemToObject(root, key_num, num_item);
    add_ok2 = cJSON_AddItemToObjectCS(root, key_name, str_item);

    // step 4: Operate - retrieve stored values using provided getters
    retrieved_num = cJSON_GetNumberValue(num_item);
    retrieved_str = cJSON_GetStringValue(str_item);

    // step 5: Validate - compute a simple validation score from object state and retrieved values
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(retrieved_str != (char *)0) + (int)(retrieved_num == 3.14);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}