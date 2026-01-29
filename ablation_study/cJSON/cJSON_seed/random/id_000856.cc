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
//<ID> 856
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool added_extracted = 0;
    cJSON_bool is_number_flag = 0;
    double detached_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize (parse JSON with an array)
    const char *json = "{\"numbers\":[10,20,30]}";
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "numbers");

    // step 3: Configure (detach the second element from the array and add it to root)
    detached = cJSON_DetachItemFromArray(arr, 1);
    added_extracted = cJSON_AddItemToObject(root, "extracted", detached);
    is_number_flag = cJSON_IsNumber(detached);

    // step 4: Operate (read the numeric value from the detached item)
    detached_value = cJSON_GetNumberValue(detached);

    // step 5: Validate (aggregate basic checks into a score)
    validation_score = (root != (cJSON *)0) + (arr != (cJSON *)0) + (detached != (cJSON *)0) + (int)added_extracted + (int)is_number_flag + (detached_value == 20.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}