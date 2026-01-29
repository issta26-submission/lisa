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
//<ID> 820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added = 0;
    cJSON_bool are_equal = 0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.42);

    // step 3: Configure
    added = cJSON_AddItemToObject(root, "value", num_item);

    // step 4: Operate
    retrieved_num = cJSON_GetObjectItem(root, "value");
    dup_num = cJSON_Duplicate(num_item, 1);
    are_equal = cJSON_Compare(num_item, dup_num, 1);
    extracted_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (retrieved_num != (cJSON *)0) + (dup_num != (cJSON *)0) + (int)are_equal + (extracted_value == 42.42) + (int)added;
    (void)validation_score;
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_num);

    // API sequence test completed successfully
    return 66;
}