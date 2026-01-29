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
//<ID> 829
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
    double value_a = 0.0;
    double value_b = 0.0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool compare_eq = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(42.0);
    num_b = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_a = cJSON_AddItemToObject(root, "alpha", num_a);
    added_b = cJSON_AddItemToObject(root, "beta", num_b);

    // step 4: Operate
    retrieved_a = cJSON_GetObjectItem(root, "alpha");
    retrieved_b = cJSON_GetObjectItem(root, "beta");
    value_a = cJSON_GetNumberValue(retrieved_a);
    value_b = cJSON_GetNumberValue(retrieved_b);
    compare_eq = cJSON_Compare(retrieved_a, retrieved_b, 1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)added_a + (int)added_b + (retrieved_a != (cJSON *)0) + (retrieved_b != (cJSON *)0) + (value_a == value_b) + (int)compare_eq + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}