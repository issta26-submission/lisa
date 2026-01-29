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
//<ID> 827
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
    cJSON_bool cmp_eq = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(3.1415);
    num_b = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(root, "pi1", num_a);
    cJSON_AddItemToObject(root, "pi2", num_b);

    // step 4: Operate
    retrieved_a = cJSON_GetObjectItem(root, "pi1");
    retrieved_b = cJSON_GetObjectItem(root, "pi2");
    val_a = cJSON_GetNumberValue(retrieved_a);
    val_b = cJSON_GetNumberValue(retrieved_b);
    cmp_eq = cJSON_Compare(retrieved_a, retrieved_b, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(val_a == val_b) + (int)cmp_eq + (printed != (char *)0) + (retrieved_a != (cJSON *)0) + (retrieved_b != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}