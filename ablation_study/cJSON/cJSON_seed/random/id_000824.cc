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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool cmp_equal = 0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(123.456);
    num2 = cJSON_CreateNumber(123.456);

    // step 3: Configure
    cmp_equal = cJSON_Compare(num1, num2, 1);
    added1 = cJSON_AddItemToObject(root, "val1", num1);
    added2 = cJSON_AddItemToObject(root, "val2", num2);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved = cJSON_GetObjectItem(root, "val1");
    extracted_value = cJSON_GetNumberValue(retrieved);

    // step 5: Validate
    validation_score = (retrieved != (cJSON *)0) + (int)cmp_equal + (printed != (char *)0) + (int)(extracted_value == 123.456);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}