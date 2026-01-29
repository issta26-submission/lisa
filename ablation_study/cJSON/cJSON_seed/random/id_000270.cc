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
//<ID> 270
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
    cJSON *min_str_item = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    cJSON_bool add_ok_text = (cJSON_bool)0;
    cJSON_bool add_ok_raw = (cJSON_bool)0;
    double setval = 0.0;
    double retrieved_val = 0.0;
    const char *retrieved_raw_cstr = (const char *)0;
    char json_buf[] = " { \"hello\" : \" world \" , \"num\" : 0 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(0.0);
    str_item = cJSON_CreateString("sample description");

    // step 3: Configure
    cJSON_Minify(json_buf);
    min_str_item = cJSON_CreateString(json_buf);
    setval = cJSON_SetNumberHelper(num_item, 123.456);
    add_ok_text = cJSON_AddItemToObject(root, "description", str_item);
    add_ok_raw = cJSON_AddItemToObject(root, "raw_json", min_str_item);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 4: Operate
    retrieved_raw = cJSON_GetObjectItem(root, "raw_json");
    retrieved_num = cJSON_GetObjectItem(root, "value");
    retrieved_raw_cstr = cJSON_GetStringValue(retrieved_raw);
    retrieved_val = cJSON_GetNumberValue(retrieved_num);
    sum_item = cJSON_CreateNumber(retrieved_val + setval);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate
    cJSON_GetObjectItem(root, "description");
    cJSON_Minify(json_buf);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}