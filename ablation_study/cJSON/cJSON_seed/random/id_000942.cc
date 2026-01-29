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
//<ID> 942
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "pi";
    const char *str_key = "message";
    const char *msg_val = "hello cJSON";
    double num_val = 3.14159265;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON_bool add_ok_num = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    char *extracted_str = (char *)0;
    double extracted_num = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root object
    root = cJSON_CreateObject();

    // step 3: Configure - create number and string items
    num_item = cJSON_CreateNumber(num_val);
    str_item = cJSON_CreateString(msg_val);

    // step 4: Operate - add items to the root using required APIs
    add_ok_num = cJSON_AddItemToObject(root, num_key, num_item);
    add_ok_str = cJSON_AddItemToObjectCS(root, str_key, str_item);

    // step 5: Validate - extract values using required getter APIs and compute a simple score
    extracted_num = cJSON_GetNumberValue(num_item);
    extracted_str = cJSON_GetStringValue(str_item);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok_num + (int)add_ok_str + (int)(extracted_str != (char *)0) + (int)(extracted_num > 0.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}