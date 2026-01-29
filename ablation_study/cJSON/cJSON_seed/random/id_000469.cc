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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *count_num = (cJSON *)0;
    cJSON *double_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool has_count = 0;
    double value = 0.0;
    double value_double = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_str = cJSON_CreateString("example");
    count_num = cJSON_CreateNumber(42.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 4: Operate
    has_count = cJSON_HasObjectItem(root, "count");
    value = cJSON_GetNumberValue((const cJSON *const)count_num);
    double_num = cJSON_CreateNumber(value * 2.0);
    cJSON_AddItemToObject(root, "count_double", double_num);

    // step 5: Validate
    printed = cJSON_Print((const cJSON *)root);
    cJSON_Minify(printed);
    value_double = cJSON_GetNumberValue((const cJSON *const)double_num);
    (void)has_count;
    (void)value;
    (void)value_double;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}