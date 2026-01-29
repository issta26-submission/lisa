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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);

    // step 2: Configure
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(stats, "pi", pi_item);
    cJSON *answer_item = cJSON_AddNumberToObject(stats, "answer", 42.0);

    // step 3: Operate & Validate
    cJSON *retrieved_pi = cJSON_GetObjectItem(stats, "pi");
    double pi_val = cJSON_GetNumberValue(retrieved_pi);
    cJSON *retrieved_answer = cJSON_GetObjectItem(stats, "answer");
    double answer_val = cJSON_GetNumberValue(retrieved_answer);
    double total = pi_val + answer_val;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(stats, "sum", total_item);
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}