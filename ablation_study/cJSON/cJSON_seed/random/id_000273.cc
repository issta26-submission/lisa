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
//<ID> 273
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
    cJSON *parsed = (cJSON *)0;
    cJSON *b_item = (cJSON *)0;
    cJSON *copied_num_item = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    char json_buf[] = "  { \"x\" : 7 , \"y\" : 13 }  \n";
    char *printed = (char *)0;
    double b_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");
    num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddItemToObject(root, "pi", num_item);

    // step 4: Operate
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    b_item = cJSON_GetObjectItem(parsed, "y");
    b_val = cJSON_GetNumberValue(b_item);
    copied_num_item = cJSON_CreateNumber(b_val);
    cJSON_AddItemToObject(root, "copied_y", copied_num_item);
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    cJSON_SetNumberHelper(retrieved_pi, cJSON_GetNumberValue(retrieved_pi) * 2.0);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}