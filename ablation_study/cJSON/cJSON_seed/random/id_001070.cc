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
//<ID> 1070
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Setup - create a numeric JSON item
    num = cJSON_CreateNumber(100.25);

    // step 3: Operate - read the numeric value and create a string JSON item
    num_value = cJSON_GetNumberValue(num);
    str = cJSON_CreateString("numeric_item");

    // step 4: Validate - compute a small validation score from results
    validation_score = (int)(num != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(num_value > 50.0);
    (void)validation_score;

    // step 5: Cleanup - delete created JSON items
    cJSON_Delete(num);
    cJSON_Delete(str);

    // step 6: API sequence test completed successfully
    return 66;
}