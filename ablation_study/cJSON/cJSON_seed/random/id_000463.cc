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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *dbl_item = (cJSON *)0;
    cJSON *has_bool_item = (cJSON *)0;
    char *printed = (char *)0;
    double num_val = 0.0;
    double dbl_val = 0.0;
    cJSON_bool has_pi = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(root, "msg", str_item);
    cJSON_AddItemToObject(root, "pi", num_item);

    // step 4: Operate
    num_val = cJSON_GetNumberValue((const cJSON *const)num_item);
    dbl_item = cJSON_CreateNumber(num_val * 2.0);
    cJSON_AddItemToObject(root, "double_pi", dbl_item);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    has_pi = cJSON_HasObjectItem((const cJSON * )root, "pi");
    has_bool_item = cJSON_CreateBool(has_pi);
    cJSON_AddItemToObject(root, "has_pi", has_bool_item);

    // step 5: Validate
    dbl_val = cJSON_GetNumberValue((const cJSON *const)dbl_item);
    (void)dbl_val;
    (void)num_val;
    (void)has_pi;
    (void)has_bool_item;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}