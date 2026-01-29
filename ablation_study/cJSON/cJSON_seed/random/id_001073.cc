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
//<ID> 1073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *num = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    const char *label_text = "answer";
    double value = 0.0;
    int validation_score = 0;

    // step 2: Setup - create a number and a string
    num = cJSON_CreateNumber(42.0);
    label = cJSON_CreateString(label_text);

    // step 3: Operate - read the number and create a derived number
    value = cJSON_GetNumberValue(num);
    num2 = cJSON_CreateNumber(value + 0.5);

    // step 4: Validate - compute a small validation score from results
    validation_score = (int)(num != (cJSON *)0) + (int)(label != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(value == 42.0) + (int)(cJSON_GetNumberValue(num2) == 42.5);
    (void)validation_score;

    // step 5: Cleanup - delete all created items
    cJSON_Delete(num);
    cJSON_Delete(num2);
    cJSON_Delete(label);

    // step 6: Finalization
    // API sequence test completed successfully
    return 66;
}