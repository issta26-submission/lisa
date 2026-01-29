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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *answer_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer_num);

    // step 2: Configure
    cJSON *pi_item = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate & Validate
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    double val_answer = cJSON_GetNumberValue(got_answer);
    cJSON *got_pi = cJSON_GetObjectItem(root, "pi");
    double val_pi = cJSON_GetNumberValue(got_pi);
    double sum = val_answer + val_pi;
    cJSON *sum_item = cJSON_AddNumberToObject(root, "sum", sum);
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}