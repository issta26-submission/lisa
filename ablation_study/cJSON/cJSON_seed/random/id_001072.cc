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
//<ID> 1072
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
    int derived = 0;
    int validation_score = 0;

    // step 2: Setup - create JSON values
    num = cJSON_CreateNumber(42.5);
    str = cJSON_CreateString("hello world");

    // step 3: Operate - read number value
    num_value = cJSON_GetNumberValue(num);

    // step 4: Validate - compute a small validation score using results
    derived = (int)(num_value * 2);
    validation_score = (int)(num != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(num_value > 40.0) + (int)(derived == 85);
    (void)validation_score;
    (void)derived;

    // step 5: Cleanup - delete created cJSON items
    cJSON_Delete(num);
    cJSON_Delete(str);

    // API sequence test completed successfully
    return 66;
}