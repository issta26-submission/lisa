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
//<ID> 1075
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    double extracted_value = 0.0;
    int validation_score = 0;
    const char *sample_text = "example";

    // step 2: Setup - create a numeric item and a string item
    num_item = cJSON_CreateNumber(42.5);
    str_item = cJSON_CreateString(sample_text);

    // step 3: Operate - extract the numeric value from the created number item
    extracted_value = cJSON_GetNumberValue(num_item);

    // step 4: Validate - compute a small validation score from the results
    validation_score = (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(extracted_value > 40.0);
    (void)validation_score;

    // step 5: Cleanup - delete created items to free resources
    cJSON_Delete(num_item);
    cJSON_Delete(str_item);

    // step 6:
    // API sequence test completed successfully
    return 66;
}