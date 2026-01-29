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
//<ID> 821
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
    cJSON *num_item_copy = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    double value = 0.0;
    cJSON_bool cmp = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    num_item_copy = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "answer_copy", num_item_copy);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "answer");
    value = cJSON_GetNumberValue(retrieved);
    cmp = cJSON_Compare(retrieved, cJSON_GetObjectItem(root, "answer_copy"), (cJSON_bool)1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved != (cJSON *)0) + (int)cmp + (printed != (char *)0) + (value == 42.0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}