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
//<ID> 1071
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
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create a number and a string JSON item
    num = cJSON_CreateNumber(123.456);
    str = cJSON_CreateString("example");

    // step 3: Configure - no container used in this sequence; prepare to operate on items
    extracted_value = cJSON_GetNumberValue(num);

    // step 4: Operate & Validate - derive a small validation score from the results
    validation_score = (int)(num != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(extracted_value > 100.0);
    (void)validation_score;

    // step 5: Cleanup - delete created items
    cJSON_Delete(num);
    cJSON_Delete(str);

    // API sequence test completed successfully
    return 66;
}