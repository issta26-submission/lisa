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
//<ID> 822
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
    cJSON *num_dup = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON *retrieved2 = (cJSON *)0;
    char *printed = (char *)0;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool compare_eq = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(42.5);
    num2 = cJSON_CreateNumber(-13.25);

    // step 3: Configure
    cJSON_AddItemToObject(root, "first", num1);
    cJSON_AddItemToObject(root, "second", num2);
    num_dup = cJSON_Duplicate(num1, 1);
    cJSON_AddItemToObject(root, "first_copy", num_dup);

    // step 4: Operate
    retrieved1 = cJSON_GetObjectItem(root, "first");
    retrieved2 = cJSON_GetObjectItem(root, "second");
    val1 = cJSON_GetNumberValue(retrieved1);
    val2 = cJSON_GetNumberValue(retrieved2);
    compare_eq = cJSON_Compare(retrieved1, num_dup, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (printed != (char *)0) + (val1 == 42.5) + (val2 < 0.0) + (int)compare_eq;
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}