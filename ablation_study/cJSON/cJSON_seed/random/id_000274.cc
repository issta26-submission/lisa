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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    cJSON *min_item = (cJSON *)0;
    cJSON *retrieved_min = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    double set_ret1 = 0.0;
    double set_ret2 = 0.0;
    char json_example[] = "  {  \"hello\" : \"world\" , \"num\" :  0 }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example-name");
    value_item = cJSON_CreateNumber(1.0);
    set_ret1 = cJSON_SetNumberHelper(value_item, 3.1415);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "name", name_item);
    add_ok2 = cJSON_AddItemToObject(root, "value", value_item);

    // step 4: Operate
    cJSON_Minify(json_example);
    min_item = cJSON_CreateString(json_example);
    add_ok3 = cJSON_AddItemToObject(root, "minified", min_item);
    set_ret2 = cJSON_SetNumberHelper(value_item, 42.0);

    // step 5: Validate
    retrieved_min = cJSON_GetObjectItem(root, "minified");
    retrieved_value = cJSON_GetObjectItem(root, "value");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}