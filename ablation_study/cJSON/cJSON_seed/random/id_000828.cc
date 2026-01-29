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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *retrieved_a = (cJSON *)0;
    cJSON *retrieved_b = (cJSON *)0;
    char *printed = (char *)0;
    double val_a = 0.0;
    double val_b = 0.0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool items_equal = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(42.0);
    num_b = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_a = cJSON_AddItemToObject(root, "a", num_a);
    added_b = cJSON_AddItemToObject(root, "b", num_b);

    // step 4: Operate
    retrieved_a = cJSON_GetObjectItem(root, "a");
    retrieved_b = cJSON_GetObjectItem(root, "b");
    val_a = cJSON_GetNumberValue(retrieved_a);
    val_b = cJSON_GetNumberValue(retrieved_b);
    items_equal = cJSON_Compare(retrieved_a, retrieved_b, 1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (printed != (char *)0) + (int)(val_a == val_b) + (int)added_a + (int)added_b + (int)items_equal;
    (void)validation_score;
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}