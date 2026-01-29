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
//<ID> 1351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    char *retrieved_str = (char *)0;
    double num_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a numeric item
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);

    // step 3: Configure - attach the number and a string to the root, duplicate the number
    cJSON_AddItemToObject(root, "answer", num);
    str_item = cJSON_AddStringToObject(root, "greeting", "hello");
    dup_num = cJSON_Duplicate(num, 0);
    cJSON_AddItemToObject(root, "answer_dup", dup_num);

    // step 4: Operate - obtain the string value and read the numeric value
    retrieved_str = cJSON_GetStringValue(str_item);
    num_val = cJSON_GetNumberValue(num);

    // step 5: Validate - compute a simple validation score from observed values
    validation_score = (int)(retrieved_str != (char *)0) + (int)(dup_num != (cJSON *)0) + (int)(num_val == 42.0);

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)retrieved_str;
    (void)num_val;
    return 66;
}