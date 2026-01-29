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
//<ID> 279
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
    cJSON *got_str = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    double set_ret = 0.0;
    double retrieved_val = 0.0;
    char *retrieved_str = (char *)0;
    char json_buf[] = "  { \"example\" : \"   spaced out   \" }  ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");
    num_item = cJSON_CreateNumber(0.0);

    // step 3: Configure
    set_ret = cJSON_SetNumberHelper(num_item, 123.456);
    add_ok_str = cJSON_AddItemToObject(root, "greeting", str_item);
    add_ok_num = cJSON_AddItemToObject(root, "value", num_item);
    cJSON_Minify(json_buf);

    // step 4: Operate
    got_str = cJSON_GetObjectItem(root, "greeting");
    got_num = cJSON_GetObjectItem(root, "value");
    retrieved_str = cJSON_GetStringValue(got_str);
    retrieved_val = cJSON_GetNumberValue(got_num);

    // step 5: Validate
    sum_item = cJSON_CreateNumber(retrieved_val + set_ret);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}