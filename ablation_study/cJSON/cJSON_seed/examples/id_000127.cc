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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.1415926535);
    cJSON_AddItemToObject(root, "pi_ref", num_pi);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 2: Configure
    cJSON *num_e = cJSON_CreateNumber(2.718281828);
    cJSON_AddItemToObject(root, "e_ref", num_e);

    // step 3: Operate & Validate
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi_ref");
    cJSON *answer_item = cJSON_GetObjectItem(root, "answer");
    cJSON *e_item = cJSON_GetObjectItem(root, "e_ref");
    double pi_val = cJSON_GetNumberValue(pi_item);
    double ans_val = cJSON_GetNumberValue(answer_item);
    double e_val = cJSON_GetNumberValue(e_item);
    double total = pi_val + ans_val + e_val;
    cJSON_AddNumberToObject(root, "sum", total);
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}