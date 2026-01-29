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
//<ID> 826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    char *printed = (char *)0;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool are_equal = 0;
    int validation_score = 0;

    // step 2: Initialize
    num1 = cJSON_CreateNumber(42.0);
    num2 = cJSON_CreateNumber(42.0);

    // step 3: Configure
    (void)0;

    // step 4: Operate
    val1 = cJSON_GetNumberValue(num1);
    val2 = cJSON_GetNumberValue(num2);
    are_equal = cJSON_Compare(num1, num2, 1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(num1);
    validation_score = (printed != (char *)0) + (int)(val1 == val2) + (int)are_equal;
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(num1);
    cJSON_Delete(num2);

    // API sequence test completed successfully
    return 66;
}