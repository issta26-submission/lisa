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
//<ID> 278
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
    cJSON *retrieved_str = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    double set_value = 0.0;
    double read_value = 0.0;
    const char *read_string = (const char *)0;
    char sample_json[] = " { \"greeting\" : \"hello world\" , \"unused\" :  42 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("example string");
    num_item = cJSON_CreateNumber(0.0);
    set_value = cJSON_SetNumberHelper(num_item, 123.456);

    // step 3: Configure
    add_ok_str = cJSON_AddItemToObject(root, "text", str_item);
    add_ok_num = cJSON_AddItemToObject(root, "value", num_item);

    // step 4: Operate
    retrieved_str = cJSON_GetObjectItem(root, "text");
    retrieved_num = cJSON_GetObjectItem(root, "value");
    read_string = cJSON_GetStringValue(retrieved_str);
    read_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    cJSON_Minify(sample_json);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}