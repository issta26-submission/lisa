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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 2: Configure
    cJSON *answer_item = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(answer_item);
    double pi_val = cJSON_GetNumberValue(pi_item);
    cJSON *total_item = cJSON_CreateNumber(answer_val + pi_val);
    cJSON_AddItemToObject(root, "total", total_item);

    // step 3: Operate & Validate
    cJSON *retrieved_total = cJSON_GetObjectItem(root, "total");
    double retrieved_val = cJSON_GetNumberValue(retrieved_total);
    (void)retrieved_val; /* use value to demonstrate flow without branching */
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}