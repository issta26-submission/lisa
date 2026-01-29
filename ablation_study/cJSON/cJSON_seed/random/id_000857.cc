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
//<ID> 857
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
    cJSON *meta = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool added_detached = 0;
    cJSON_bool is_number_flag = 0;
    double count_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize (parse a JSON containing an array and metadata)
    root = cJSON_Parse("{\"numbers\":[1,2,3],\"meta\":{\"count\":3}}");

    // step 3: Configure (locate the array and detach an element)
    arr = cJSON_GetObjectItem(root, "numbers");
    meta = cJSON_GetObjectItem(root, "meta");
    detached = cJSON_DetachItemFromArray(arr, 1);
    is_number_flag = cJSON_IsNumber(detached);
    added_detached = cJSON_AddItemToObject(root, "detached", detached);

    // step 4: Operate (inspect metadata)
    count_item = cJSON_GetObjectItem(meta, "count");
    count_value = cJSON_GetNumberValue(count_item);

    // step 5: Validate (accumulate simple checks into a score)
    validation_score = (root != (cJSON *)0) + (arr != (cJSON *)0) + (detached != (cJSON *)0) + (int)is_number_flag + (int)added_detached + (count_value == 3.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}