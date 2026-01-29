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
//<ID> 271
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
    cJSON *got_str_item = (cJSON *)0;
    cJSON *got_num_item = (cJSON *)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    char json_buf[] = "  {  \"unused\"  :  \" spaced value \" , \"x\" :  0  }  ";
    double num_before = 0.0;
    double num_after = 0.0;
    char *retrieved_str_value = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("sample text");
    num_item = cJSON_CreateNumber(3.5);
    num_before = cJSON_GetNumberValue(num_item);

    // step 3: Configure
    add_ok_str = cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_SetNumberHelper(num_item, 99.25);
    add_ok_num = cJSON_AddItemToObject(root, "answer", num_item);

    // step 4: Operate
    cJSON_Minify(json_buf);
    got_str_item = cJSON_GetObjectItem(root, "greeting");
    got_num_item = cJSON_GetObjectItem(root, "answer");
    retrieved_str_value = cJSON_GetStringValue(got_str_item);
    num_after = cJSON_GetNumberValue(got_num_item);

    // step 5: Validate
    (void)retrieved_str_value;
    (void)num_before;
    (void)add_ok_str;
    (void)add_ok_num;
    (void)num_after;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}